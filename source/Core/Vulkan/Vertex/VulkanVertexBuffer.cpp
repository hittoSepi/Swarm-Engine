#include "pch.h"

uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}


/////////////////////////////////////////////
//			Base VulkanBuffer
/////////////////////////////////////////////
VulkanBuffer::VulkanBuffer(const VulkanDevices& devices, VulkanCommandPool* commandPool) :
	devices(devices), commandPool(commandPool)
{

}


VulkanBuffer::~VulkanBuffer()
{
	vkDestroyBuffer(devices.vkDevice, buffer, nullptr);
	vkFreeMemory(devices.vkDevice, bufferMemory, nullptr);
}


void VulkanBuffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool->getCommandPool();
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(devices.vkDevice, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	{
		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0; // Optional
		copyRegion.dstOffset = 0; // Optional
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
	}
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;
	auto graphicsQue = devices.vulkanDevice->getGraphicsQueue();
	vkQueueSubmit(graphicsQue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQue);
}


void VulkanBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	auto device = devices.vkDevice;
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(devices.vkPhysicalDevice, memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}




/////////////////////////////////////////////
//			VulkanVerteBuffer
/////////////////////////////////////////////
VulkanVertexBuffer::VulkanVertexBuffer(const VulkanDevices& devices, VulkanCommandPool* commandPool) :
	VulkanBuffer(devices, commandPool)
{
	LogInfo("");
}


VulkanVertexBuffer::VulkanVertexBuffer(const VulkanDevices& devices, VulkanCommandPool* commandPool, const std::vector<Vertex3>& vertices) :
	VulkanBuffer(devices, commandPool), vertices(vertices)
{
	LogInfo("");
}


void VulkanVertexBuffer::create()
{
	auto device = devices.vkDevice;
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, bufferMemory);
	copyBuffer(stagingBuffer, buffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}




/////////////////////////////////////////////
//			VulkanIndexBuffer
/////////////////////////////////////////////
VulkanIndexbuffer::VulkanIndexbuffer(const VulkanDevices& devices, VulkanCommandPool* commandPool) :
	VulkanBuffer(devices, commandPool)
{

}

VulkanIndexbuffer::VulkanIndexbuffer(const VulkanDevices& devices, VulkanCommandPool* commandPool, const std::vector<uint32_t>& indices) :
	VulkanBuffer(devices, commandPool), indices(indices)
{

}


void VulkanIndexbuffer::create()
{
	VkDevice device = devices.vkDevice;
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, bufferMemory);

	copyBuffer(stagingBuffer, buffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}



/////////////////////////////////////////////
//			VulkanUniformBuffer
/////////////////////////////////////////////
VulkanUniformBuffer::VulkanUniformBuffer(const VulkanDevices& devices) :
	devices(devices)
{
}

VulkanUniformBuffer::~VulkanUniformBuffer()
{
	auto device = devices.vkDevice;
	auto api = devices.vulkanApi;
	auto swap = static_cast<VulkanSwapChain*>(api->getSwapChain());
	
	for (size_t i = 0; i < swap->getSize(); i++) {
		vkDestroyBuffer(device, uniformBuffers[i], nullptr);
		vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
	}
}


void VulkanUniformBuffer::create(mvpUniform data_)
{

	data = data_;
	LogInfo(sizeof(data));
	auto api = devices.vulkanApi;
	auto swap = static_cast<VulkanSwapChain*>(api->getSwapChain());
	auto size = swap->getSize();

	VkDeviceSize bufferSize = sizeof(data);

	uniformBuffers.resize(size);
	uniformBuffersMemory.resize(size);

	for (size_t i = 0; i < size; i++) {
		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}


void VulkanUniformBuffer::update(VulkanCommandPool* commandPool, uint32_t currentImage, mvpUniform newData)
{
	void* data;
	vkMapMemory(devices.vkDevice, uniformBuffersMemory[currentImage], 0, sizeof(newData), 0, &data);
	memcpy(data, &newData, sizeof(newData));
	vkUnmapMemory(devices.vkDevice, uniformBuffersMemory[currentImage]);
}


void VulkanUniformBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	auto device = devices.vkDevice;
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(devices.vkPhysicalDevice, memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

