#include "pch.h"
#include "Core/Vulkan/VulkanCommandPool.h"



const std::vector<Vertex3> vertices = {
	{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};

const std::vector<uint32_t> indices = {
	0, 1, 2, 2, 3, 0
};




VkCommandBuffer beginSingleTimeCommands(VkDevice device, VulkanCommandPool* commandPool) {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool->getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}


void endSingleTimeCommands(VkDevice device, VkQueue graphicsQueue, VulkanCommandPool* commandPool, VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

	
    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(device, commandPool->getCommandPool(), 1, &commandBuffer);
}

VulkanCommandPool::VulkanCommandPool(const VulkanDevices& devices_,VulkanRenderingPipeline* pipeline_, const size_t &framebufferCount) :
	devices(devices_), pipeline(pipeline_), framebufferCount(framebufferCount)
{

	LogInfo("");

	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = devices.vulkanDevice->findQueueFamilies(devices.vkPhysicalDevice).graphicsFamily.value();
	poolInfo.flags = 0; // Optional

	if (vkCreateCommandPool(devices.vkDevice, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

void VulkanCommandPool::createBuffers() {

	
	buffers.resize(framebufferCount);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)buffers.size();

	if (vkAllocateCommandBuffers(devices.vkDevice, &allocInfo, buffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	vertexBuffer = new VulkanVertexBuffer(devices, this);
	vertexBuffer->addVertices(vertices);
	vertexBuffer->create();

	indexBuffer = new VulkanIndexbuffer(devices, this, indices);
	indexBuffer->create();
	
	auto descSets = pipeline->getDescriptorSets();
	auto pipelayout = pipeline->getVkPipelineLayout();
	
	for (size_t index = 0; index < buffers.size(); index++) {

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(buffers[index], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = pipeline->getRenderPass()->getVkRenderPass();
		renderPassInfo.framebuffer = pipeline->getFramebuffers()[index];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = pipeline->getVulkanSwapChain()->getExtent();

		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(buffers[index], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(buffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVkPipeline());


				VkBuffer vertexBuffers[] = { vertexBuffer->getBuffer() };
				VkDeviceSize offsets[] = { 0 };
				
				vkCmdBindVertexBuffers(buffers[index], 0, 1, vertexBuffers, offsets);
				vkCmdBindIndexBuffer(buffers[index], indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);

				vkCmdBindDescriptorSets(buffers[index], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelayout, 0, 1, &descSets[index], 0, nullptr);
		
				vkCmdDrawIndexed(buffers[index], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);


		vkCmdEndRenderPass(buffers[index]);
		if (vkEndCommandBuffer(buffers[index]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}

}


void VulkanCommandPool::quit()
{
	vkDestroyCommandPool(devices.vkDevice, commandPool, nullptr);

	
	delete vertexBuffer;
	delete indexBuffer;
}

void VulkanCommandPool::begin(int index_)
{


}


void VulkanCommandPool::end()
{

}

void VulkanCommandPool::excecute(VulkanRenderingPipeline* pipeline)
{


}