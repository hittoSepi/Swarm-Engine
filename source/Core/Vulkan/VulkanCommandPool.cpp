#include "pch.h"
#include "Core/Vulkan/VulkanCommandPool.h"

VulkanCommandPool::VulkanCommandPool(const VulkanDevices& devices_, VulkanRenderingPipeline* pipeline_, size_t framebufferCount) :
	devices(devices_), pipeline(pipeline_)
{
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = devices.vulkanDevice->findQueueFamilies(devices.vkPhysicalDevice).graphicsFamily.value();
	poolInfo.flags = 0; // Optional

	if (vkCreateCommandPool(devices.vkDevice, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}

	buffers.resize(framebufferCount);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)buffers.size();

	if (vkAllocateCommandBuffers(devices.vkDevice, &allocInfo, buffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}


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
		vkCmdDraw(buffers[index], 3, 1, 0, 0);

		vkCmdEndRenderPass(buffers[index]);

		if (vkEndCommandBuffer(buffers[index]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}

}


void VulkanCommandPool::quit()
{
	vkDestroyCommandPool(devices.vkDevice, commandPool, nullptr);
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