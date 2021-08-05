#include "pch.h"

#include "Core/Vulkan/Render/VulkanRenderPass.h"


VkRenderPassCreateInfo::VkRenderPassCreateInfo(VulkanSubPass pass)
{
	sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	attachmentCount = 1;
	pAttachments = this;

	
	// SubPasses
	std::vector<VkRenderPassCreateInfo> vkPassInfos;
	for (auto subpass : subPasses)
	{
		auto pass = static_cast<VkRenderPassCreateInfo>(subpass);
		vkPassInfos.push(pass);
	}

	pSubpasses = subPasses.data();
}


/// Render SubPass
VulkanSubPass::VulkanSubPass(std::string name):
	VulkanRenderPass(name)
{
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
}




/// Render Pass
VulkanRenderPass::VulkanRenderPass(std::string name):
	RenderPass(name), VkRenderPassCreateInfo()
{
	// VkRenderPassCreateInfo
	sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	attachmentCount = 1;
	pAttachments = this;


	// SubPasses
	std::vector<VkRenderPassCreateInfo> vkPassInfos;
	for(auto subpass: subPasses)
	{
		auto pass = static_cast<VkRenderPassCreateInfo>(subpass);
		vkPassInfos.push(pass);
	}
	
	pSubpasses = subPasses.data();

	
	// VkAttachmentDescription
	format = VK_FORMAT_B8G8R8A8_SRGB;
	samples = VK_SAMPLE_COUNT_1_BIT;
	loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	
}


void VulkanRenderPass::addSubPass(SubPass *subpass)
{
	auto pass = (VulkanSubPass*)(subpass);
	subPasses.push_back(pass);
}

