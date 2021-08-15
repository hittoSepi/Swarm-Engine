#include "pch.h"

VulkanFrameBuffer::VulkanFrameBuffer(const VulkanDevices& devices, VkRenderPass renderPass, const VkImageView* attachmentViews, int width, int height):
	devices(devices), width(width), height(height)
{
	
	
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachmentViews;
    framebufferInfo.width = width;
    framebufferInfo.height = height;
    framebufferInfo.layers = 1;
}

void VulkanFrameBuffer::quit()
{
	vkDestroyFramebuffer(devices.vkDevice, buffer, nullptr);
}