#include "pch.h"
#include "Core/Vulkan/RenderPasses/VulkanSimpleRenderPass.h"


VulkanSimpleRenderPass::VulkanSimpleRenderPass(VulkanDevices& devices_):
	VulkanRenderPass("SimpleRenderPass"), devices(devices_)
{

	VkAttachmentDescription colorAttachment{};
	colorAttachment.format =  VK_FORMAT::RGBA8;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	
	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	
	renderPassCreateInfo = VkRenderPassCreateInfo();
	renderPassCreateInfo.sType = VK_CI::SW_VULKAN_RENDER_PASS;
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pAttachments = &colorAttachment;
	renderPassCreateInfo.pSubpasses = &subpass;

	vkCreateRenderPass(devices.vkDevice, &renderPassCreateInfo, nullptr, &renderPass);
}