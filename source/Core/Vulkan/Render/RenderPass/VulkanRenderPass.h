#pragma once

class VulkanSubPass;

#include "Core/Rendering/RenderPass.h"

/// <summary>
/// <c> VulkanRenderPass </c>
/// </summary>
class VulkanRenderPass : public RenderPass
{
public:
	virtual ~VulkanRenderPass()
	{
	}
	
	std::vector<VulkanSubPass*> getSubPasses() { return subPasses; }

	virtual void addAttachment(VulkanAttachment *attach);
	virtual void addReference(VulkanAttachmentReference *ref);
	virtual void addSubPassDependency(VulkanSubPassDependency *dep);
	virtual void addSubPass(VulkanSubPass* pass);

	virtual void loadAttachments()	= 0;
	virtual void loadReferences()	= 0;
	virtual void loadSubPassDeps()	= 0;
	virtual void loadSubPasses()	= 0;

	void buildPass()
	{
		
		// prepare attachments
		loadAttachments();

		// get references
		loadReferences();

		// load subpass dependencies
		loadSubPassDeps();

		// load subpass descriptions
		loadSubPasses();
	}
protected:
	
	VulkanRenderPass(std::string name);
	VkRenderPassCreateInfo						renderPassCreateInfo;
	VkRenderPass								renderPass;
	
	std::vector<VulkanAttachment*>				attachments;
	std::vector<VulkanAttachmentReference*>		attachmentReferences;
	std::vector<VulkanSubPassDependency*>		subPassDependencies;
	std::vector<VulkanSubPass*>					subPasses;		
	
	/*


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