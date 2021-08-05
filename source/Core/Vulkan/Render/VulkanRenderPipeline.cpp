#include "pch.h"

#include "Core/Rendering/Viewport.h"
#include "Core/Vulkan/Render/VulkanRenderPipeline.h"
#include "Core/Vulkan/Render/VulkanViewport.h"




VulkanRenderingPipeline::VulkanRenderingPipeline(VulkanApi* api_) :
	RenderingPipeline("Vulkan RenderingPipeline HD")
{
	LogInfo("");
	setRenderingApi((VulkanApi*)api_);


	vkViewports.clear();
	vkScissors.clear();
	viewports.clear();
	scissors.clear();

	
	LogVerbose("end.")
}


void VulkanRenderingPipeline::init()
{
	LogInfo("");
	auto api_ = getVulkanApi();
	
	// create swap chain
	VulkanSwapChain* swapchain = new VulkanSwapChain(api_);
	swapchain->init();
	// create viewport
	auto swapDims = swapchain->getDimensions();
	setSwapChain((VulkanSwapChain*)swapchain);


	addViewport(swapDims, float2(0.0f, 1.0f));
	addScissor(swapDims);
	createViewports();

	if (vkCreatePipelineLayout(api_->getVkDevice(), &createInfos.pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
    }

	/*
	*Shader::CreateOptions opts;
	opts.name = "Basic";
	opts.filename = "basic";
	opts.device = device;

	testshader = VulkanShader::create(opts);
	*/

	LogVerbose("end.")
}


void VulkanRenderingPipeline::quit()
{
	LogInfo("");
	auto dev = getVulkanApi()->getVkDevice();
	auto sc = dynamic_cast<VulkanSwapChain*>(swapChain);
	
	sc->quit();
	delete sc;

	vkDestroyPipelineLayout(dev, pipelineLayout, nullptr);
	
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

void VulkanRenderingPipeline::setSwapChain(SwapChain* swapchain_)
{
	LogInfo("");
	swapChain = (VulkanSwapChain*)swapchain_;
}

void VulkanRenderingPipeline::setRenderingApi(RenderingApi* api_)
{
	LogInfo("");
	api = (VulkanApi*)api_;
}

void VulkanRenderingPipeline::createViewports()
{
	LogInfo("");
	vkViewports.clear();
	vkScissors.clear();

	for(auto scissor: scissors) {
		vkScissors.push_back(scissorToVkRect(static_cast<VulkanScissor*>(scissor)));
	}


	for(auto view: viewports)
	{
		vkViewports.push_back(viewportToVkViewport(dynamic_cast<VulkanViewport*>(view)));
	}

/*	createInfos.viewportState.viewportCount = (int)vkViewports.size();
	createInfos.viewportState.scissorCount = (int)vkScissors.size();
	createInfos.viewportState.pViewports = vkViewports.data();
	createInfos.viewportState.pScissors = vkScissors.data();
	*/
}

void* VulkanRenderingPipeline::getSwapChain()
{
	LogInfo("");
	return (VulkanSwapChain*)swapChain;
}



// Add Viewports to pipeline
void VulkanRenderingPipeline::addViewport(Viewport* viewport)
{
	LogInfo("");
	if (viewports.size() < MAX_VIEWPORT) {
		viewports.push_back(dynamic_cast<VulkanViewport*>(viewport));
	}
	else {
		LogWarning("Viewports full. Deleting pointer.");
		delete dynamic_cast<VulkanViewport*>(viewport);
	}
}

void VulkanRenderingPipeline::addViewport(const iRect& view_area, const float2& depth)
{
	addViewport((VulkanViewport*) new VulkanViewport(view_area, depth));
}


void VulkanRenderingPipeline::addScissor(const iRect& view_area)
{
	addScissor(new VulkanScissor(view_area));
}


// Add scissors to pipeline
void VulkanRenderingPipeline::addScissor(Scissor* scissorArea)
{
	LogInfo("");
	if (scissors.size() < MAX_VIEWPORT) {
		scissors.push_back((VulkanScissor*)(scissorArea));
	}
	else {
		LogWarning("Scissors full! Deleting pointer.");
		delete scissorArea;
	}

}


VkRect2D VulkanRenderingPipeline::scissorToVkRect(VulkanScissor* rect)
{
	auto y = static_cast<VkRect2D*>(rect);
	return VkRect2D(*y);
}

VkViewport VulkanRenderingPipeline::viewportToVkViewport(VulkanViewport* view)
{
	auto v = static_cast<VulkanViewport*>(view);
	return VkViewport(*v);
}

