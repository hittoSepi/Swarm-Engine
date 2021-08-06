#pragma once
#include "pch.h"


/// <summary>
/// Physically Based Shader renderpass
/// </summary>
class VulkanPBSRenderPass : public VulkanRenderPass
{
public:

	// insert all you need for creation in here
	class RequiredData
	{
	public:

		std::string name = "PBS RenderPass";

		VulkanDevices 									devices;
		
		std::vector<VulkanAttachmentReference*>			inputAttachmentReferences;
		std::vector<VulkanAttachmentReference*>			colorAttachmentReferences;
		std::vector<VulkanSubPassDependency*>			subPassDependencies;
		std::vector<VulkanAttachment*>					colorAttachments;

		std::vector<VkInputAttachmentAspectReference>	vkInputAttachmentAspectReferences;
		std::vector<VkAttachmentDescription>			vkColorAttachments;
		std::vector<VkAttachmentReference>				vkAttachmentReferences;
		std::vector<VkSubpassDependency>				vkSubPassDependencies;
		std::vector<VkAttachmentReference>				vkColorAttachmentsReferences;


		VkAttachmentDescription							depthBuffer;
		VkAttachmentReference							depthBufferRef;

		
		VulkanSubPass::CreateOptions subPassOption =
			VulkanSubPass::CreateOptions(BIND::SW_VULKAN_GRAPHICS,
				depthBufferRef,
				std::vector<VkAttachmentReference>(),
				std::vector<VkAttachmentReference>(),
				std::vector<VkAttachmentReference>(),
				std::vector<uint32_t>());


		std::vector<VkSubpassDescription> vkSubpassDescriptions;
		VkRenderPassCreateInfo renderPassCreateInfo;

		RequiredData()
		{
			renderPassCreateInfo.sType				= VK_CI::SW_VULKAN_RENDER_PASS;
			renderPassCreateInfo.flags				= 0;
			renderPassCreateInfo.pNext				= nullptr;

			renderPassCreateInfo.dependencyCount	= vkSubPassDependencies.size();
			renderPassCreateInfo.pDependencies		= vkSubPassDependencies.data();

			renderPassCreateInfo.attachmentCount	= vkColorAttachments.size();
			renderPassCreateInfo.pAttachments		= vkColorAttachments.data();

			std::vector<VulkanSubPass*>				subpasses;
			for (auto pass : subpasses)
			{
				vkSubpassDescriptions.push_back(pass->getVkSubpass());
			}

			renderPassCreateInfo.subpassCount = vkSubpassDescriptions.size();
			renderPassCreateInfo.pSubpasses = vkSubpassDescriptions.data();
			
			//renderPass = vkCreateRenderPass(info.device, &rp_info, NULL, &info.render_pass);


		}
	};


	VulkanPBSRenderPass(RequiredData data) :
		VulkanRenderPass(data.name)
	{
		
	}


	virtual ~VulkanPBSRenderPass()
	{
	}

	RequiredData getRequiredData()  { return requiredData; }

protected:
	RequiredData requiredData;
	
	std::vector<VkInputAttachmentAspectReference>	vkInputAttachmentAspectReferences;
	std::vector<VkAttachmentDescription>			vkColorAttachments;
	std::vector<VkAttachmentReference>				vkAttachmentReferences;
	std::vector<VkSubpassDependency>				vkSubPassDependencies;
	std::vector<VkAttachmentReference>				vkColorAttachmentsReferences;


	void loadAttachments()	override;
	void loadReferences()	override;
	void loadSubPassDeps()	override;
	void loadSubPasses()	override;
	
};
