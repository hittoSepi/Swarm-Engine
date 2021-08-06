#pragma once
#include "VulkanRenderPass.h"


class VulkanSubPass: public VulkanRenderPass
{
public:
	class CreateOptions
	{
	public:
		std::string	name = "VulkanSubPass";
		CreateOptions(
			VkPipelineBindPoint					bindPoint,
			VkAttachmentReference				depthStencilAttachmentReference,
			std::vector<VkAttachmentReference>	colorBufferAttachmentReferences,
			std::vector<VkAttachmentReference>	inputAttachmentReferences,
			std::vector<VkAttachmentReference>	resolveAttachmentReferences,
			std::vector<uint32_t>				preserveAttachmentsReferences
			)
			
		{
			subpass.flags					= 0;
			subpass.pipelineBindPoint		= bindPoint;
			subpass.inputAttachmentCount	= inputAttachmentReferences.size();
			subpass.pInputAttachments		= inputAttachmentReferences.data();
			subpass.colorAttachmentCount	= colorBufferAttachmentReferences.size();
			subpass.pColorAttachments		= colorBufferAttachmentReferences.data();
			subpass.pResolveAttachments		= resolveAttachmentReferences.data();
			subpass.pDepthStencilAttachment = &depthStencilAttachmentReference;
			subpass.preserveAttachmentCount = preserveAttachmentsReferences.size();
			subpass.pPreserveAttachments	= preserveAttachmentsReferences.data();
		}
		std::vector<VulkanAttachment>	attachments;
		VkSubpassDescription			subpass;
	};

	VulkanSubPass(CreateOptions options)
		:VulkanRenderPass(options.name)
	{
		subpass = options.subpass;
	}
	
	~VulkanSubPass() { }

	VkSubpassDescription getVkSubpass() { return subpass; }
private:
	VkSubpassDescription subpass;
	

};


/*
 * 
struct VulkanRenderPassCreateInfo {
	VulkanStructureType               sType;
    const void*                       pNext;
    VkRenderPassCreateFlags           flags;
    uint32_t                          attachmentCount;
    const VkAttachmentDescription*    pAttachments;
    uint32_t                          subpassCount;
    const VkSubpassDescription*       pSubpasses;
    uint32_t                          dependencyCount;
    const VkSubpassDependency*        pDependencies;

	VkRenderPassCreateInfo GetVulkanSubPassData
};


 */