#include "pch.h"
#include "Core/Vulkan/VulkanRenderPipeline.h"


VulkanRenderingPipeline::VulkanRenderingPipeline(VulkanDevice* device):
	RenderingPipeline("Vulkan RenderingPipeline HD", device)
{
	LogInfo("");


	LogVerbose("end.")
}


void VulkanRenderingPipeline::init()
{
	LogInfo("");
	Shader::CreateOptions opts;
	opts.name = "Basic";
	opts.filename = "basic";
	opts.device = device;
	
	testshader = VulkanShader::create(opts);
	

	LogVerbose("end.")
}


void VulkanRenderingPipeline::quit()
{
	LogInfo("");
	delete testshader;

	LogVerbose("end.")
}


void VulkanRenderingPipeline::update()
{
	
}


void VulkanRenderingPipeline::render()
{
	
}

void VulkanRenderingPipeline::resize(uint32_t width, uint32_t height)
{
	
}

