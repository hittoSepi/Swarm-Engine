#pragma once


class VulkanSubPass;



class VulkanRenderPass : public RenderPass
{
public:


	VulkanRenderPass(std::string name);

	void addSubPass(SubPass* pass) override;



protected:
	VkRenderPass				renderPass;
	VkPipelineLayout			pipelineLayout;
	std::vector<VulkanSubPass*>	subPasses;
	/*
	class CreateInfos {
	public:
		CreateInfos(std::vector<VulkanSubPass*> subpasses):
		{
			std::vector<VkSubpassDescription> passes;
			for(VulkanSubPass pass: subpasses)
			{
				auto p = static_cast<VkSubpassDescription>(pass);


				passes.push_back(p);
			}
			pSubpasses = passes.data();
			sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

		}

		VkRenderPassCreateInfo renderPassInfo
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			nullptr,
			0,
			1,
			&colorAttachment,

		};

		VkAttachmentDescription colorAttachment
		{
			0,
			VK_FORMAT_B8G8R8A8_SRGB,
			VK_SAMPLE_COUNT_1_BIT,
			VK_ATTACHMENT_LOAD_OP_CLEAR,
			VK_ATTACHMENT_STORE_OP_STORE,
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			VK_ATTACHMENT_STORE_OP_DONT_CARE,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
		};
	};
	*/
};

class VulkanSubPass: public VulkanRenderPass
{
public:
	VulkanSubPass(std::string name);
	~VulkanSubPass();

	VkSubpassDescription subpass
	{
		0,
		VK_PIPELINE_BIND_POINT_GRAPHICS
	};


	VkAttachmentReference colorAttachmentRef
	{
		0,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};

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