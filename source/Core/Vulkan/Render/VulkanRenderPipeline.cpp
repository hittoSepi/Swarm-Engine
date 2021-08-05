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




	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	//VkPipelineDynamicStateCreateInfo dynamicStateCI = vks::initializers::pipelineDynamicStateCreateInfo(dynamicStateEnables);
	//std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages;

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

	auto sc = dynamic_cast<VulkanSwapChain*>(swapChain);
	sc->quit();
	delete sc;

	
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

	std::vector<Scissor*>::iterator sciIt = scissors.begin();
	while (sciIt != scissors.end()) {
		const auto item = (VulkanScissor*)sciIt._Ptr;
		auto sci = scissorToVkRect(item);
		vkScissors.push_back(sci);
		sciIt++;
	}

	std::vector<Viewport*>::iterator viewIt = viewports.begin();
	while (viewIt != viewports.end()) {
		const auto item = (VulkanViewport*)viewIt._Ptr;
		auto vp = viewportToVkViewport(item);
		vkViewports.push_back(vp);
		sciIt++;
	}

	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = (int)vkViewports.size();
	viewportState.scissorCount = (int)vkScissors.size();
	viewportState.pViewports = vkViewports.data();
	viewportState.pScissors = vkScissors.data();

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

