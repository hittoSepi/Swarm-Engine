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

		VulkanDevices 									devices = VulkanDevices();
		
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
			depthBuffer = VkAttachmentDescription();
			depthBufferRef = VkAttachmentReference();
		}

		
		RequiredData(VulkanDevices& devices):
			devices(devices)
		{
			depthBuffer		= VkAttachmentDescription(); 
			depthBufferRef	= VkAttachmentReference();
			//renderPass = vkCreateRenderPass(info.device, &rp_info, NULL, &info.render_pass);
		}


		void createRenderPass()
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
		
	
private:
	};


	static VulkanPBSRenderPass create(VulkanDevices &devices
			/*std::vector<VulkanAttachmentReference*>			inputAttachmentReferences_,
			std::vector<VulkanAttachmentReference*>			colorAttachmentReferences_,
			std::vector<VulkanSubPassDependency*>			subPassDependencies_,
			std::vector<VulkanAttachment*>					colorAttachments_,
			std::vector<VkInputAttachmentAspectReference>	vkInputAttachmentAspectReferences_,
			std::vector<VkAttachmentDescription>			vkColorAttachments_,
			std::vector<VkAttachmentReference>				vkAttachmentReferences_,
			std::vector<VkSubpassDependency>				vkSubPassDependencies_,
			std::vector<VkAttachmentReference>				vkColorAttachmentsReferences_,
			VkAttachmentDescription							depthBuffer_,
			VkAttachmentReference							depthBufferRef_*/);

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
