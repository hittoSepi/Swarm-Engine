#pragma once


class VulkanPBSRenderPass : public VulkanRenderPass
{
public:
	class RequiredData
	{
	public:

		std::string name = "PBS RenderPass";

		std::vector<VulkanAttachmentReference*> inputAttachmentReferences;
		std::vector<VulkanAttachmentReference*> colorAttachmentReferences;
		std::vector<VulkanSubPassDependency*> subPassDependencies;
		std::vector<VulkanAttachment*> colorAttachments;

		std::vector<VkInputAttachmentAspectReference>	vkInputAttachmentAspectReferences;
		std::vector<VkAttachmentDescription>		vkColorAttachments;
		std::vector<VkAttachmentReference>			vkAttachmentReferences;
		std::vector<VkSubpassDependency>			vkSubPassDependencies;
		std::vector<VkAttachmentReference>			vkColorAttachmentsReferences;


		VkAttachmentDescription		depthBuffer;
		VkAttachmentReference		depthBufferRef;

		VulkanSubPass::CreateOptions subPassOption =
			VulkanSubPass::CreateOptions(VK_PIPELINE_BIND_POINT_COMPUTE,
				depthBufferRef,
				std::vector<VkAttachmentReference>(),
				std::vector<VkAttachmentReference>(),
				std::vector<VkAttachmentReference>(),
				std::vector<uint32_t>());


		std::vector<VkSubpassDescription> vkSubpassDescriptions;
		VkRenderPassCreateInfo renderPassCreateInfo;

		RequiredData()
		{


			renderPassCreateInfo.sType = VK_CI::SW_VULKAN_RENDER_PASS;
			renderPassCreateInfo.flags = 0;
			renderPassCreateInfo.pNext = nullptr;

			renderPassCreateInfo.dependencyCount = vkSubPassDependencies.size();
			renderPassCreateInfo.pDependencies = vkSubPassDependencies.data();

			renderPassCreateInfo.attachmentCount = vkColorAttachments.size();
			renderPassCreateInfo.pAttachments = vkColorAttachments.data();

			std::vector<VulkanSubPass*> passes;
			for (auto pass : passes)
			{
				vkSubpassDescriptions.push_back(pass->getVkSubpass());
			}

			renderPassCreateInfo.subpassCount = vkSubpassDescriptions.size();
			renderPassCreateInfo.pSubpasses = vkSubpassDescriptions.data();
			//
			//		renderPass = vkCreateRenderPass(info.device, &rp_info, NULL, &info.render_pass);


		}
	};


	VulkanPBSRenderPass(RequiredData data) :
		VulkanRenderPass(data.name)
	{
	}


	virtual ~VulkanPBSRenderPass()
	{
	}


	void buildPass() override
	{
		// load attachments
		loadAttachments();

		// load references
		loadReferences();

		// load subpass dependencies
		loadSubPassDeps();

		// load sub passes
		loadSubPasses();
	}

protected:
	std::vector<VkInputAttachmentAspectReference>	vkInputAttachmentAspectReferences;
	std::vector<VkAttachmentDescription>			vkColorAttachments;
	std::vector<VkAttachmentReference>				vkAttachmentReferences;
	std::vector<VkSubpassDependency>				vkSubPassDependencies;
	std::vector<VkAttachmentReference>				vkColorAttachmentsReferences;


	void loadAttachments() {


	}

	void loadReferences();

	void loadSubPassDeps();

	void loadSubPasses();



};
