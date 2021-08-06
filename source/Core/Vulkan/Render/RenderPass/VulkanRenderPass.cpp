#include "pch.h"
#include "Core/Vulkan/Render/RenderPass/VulkanSubPass.h"

/// Render Pass
VulkanRenderPass::VulkanRenderPass(std::string name):
	RenderPass(name)
{
	
}


void VulkanRenderPass::addSubPass(VulkanSubPass* subpass)
{
	subPasses.push_back(subpass);
}


void VulkanRenderPass::addSubPassDependency(VulkanSubPassDependency* dep)
{
	subPassDependencies.push_back(dep);
}


void VulkanRenderPass::addAttachment(VulkanAttachment *attach)
{
	attachments.push_back(attach);
}


void VulkanRenderPass::addReference(VulkanAttachmentReference *ref)
{
	attachmentReferences.push_back(ref);
}