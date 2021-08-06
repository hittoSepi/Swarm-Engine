#pragma once


/// <summary>
/// VulkanSubPassDependency
/// </summary>
class VulkanSubPassDependency
{
public:
	VulkanSubPassDependency(uint32_t srcSubpass, uint32_t dstSubpass,
		VkPipelineStageFlags srcStageMask = 0, VkPipelineStageFlags dstStageMask = 0,
		VkAccessFlags srcAccessMask = 0, VkAccessFlags dstAccessMask  = 0,
		VkDependencyFlags dependencyFlags = 0
	)
	{
		subpass_dependency.srcSubpass			= srcSubpass;
		subpass_dependency.dstSubpass			= dstSubpass;
		subpass_dependency.srcStageMask			= srcStageMask;
		subpass_dependency.dstStageMask			= dstStageMask;
		subpass_dependency.srcAccessMask		= srcAccessMask;
		subpass_dependency.dstAccessMask		= dstAccessMask;
		subpass_dependency.dependencyFlags		= dependencyFlags;
	}

	static VulkanSubPassDependency* pipelineColorAttachment()
	{
		return new VulkanSubPassDependency(
			VK_SUBPASS_EXTERNAL, 
			0,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			0,
			VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			0);
	} 
	
	VkSubpassDependency subpass_dependency {};
};
