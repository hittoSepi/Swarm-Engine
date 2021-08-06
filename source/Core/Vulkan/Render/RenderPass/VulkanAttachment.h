#pragma once


/// <summary>
/// VulkanAttachment
/// </summary>
class VulkanAttachment
{
public:
	VulkanAttachment(VkFormat format, VkSampleCountFlagBits numSamples, VkImageLayout initialLayout, VkImageLayout finalLayout)
	{
		attachment.format			= format;
		attachment.samples			= numSamples;
		attachment.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachment.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
		attachment.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachment.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachment.initialLayout	= initialLayout;
		attachment.finalLayout		= finalLayout;
		attachment.flags			= 0;
	}

	VkAttachmentDescription getAttachment() { return attachment; }
protected:
	VkAttachmentDescription attachment;
};
