#include "pch.h"
#include "Core/Vulkan/RenderPasses/VulkanPBSRenderPass.h"

VulkanPBSRenderPass VulkanPBSRenderPass::create(VulkanDevices &devices)
{

	VulkanPBSRenderPass::RequiredData requiredData;
	requiredData.devices = devices; 

	
	
	
	VulkanPBSRenderPass pbs = VulkanPBSRenderPass(requiredData);
	

	return pbs;
}