#include "pch.h"

const int MAX_FRAMES_IN_FLIGHT = 2;


VulkanRenderingPipeline::VulkanRenderingPipeline(CreateOptions createOptions) :
	RenderingPipeline("Vulkan RenderingPipeline HD"), createOptions(createOptions)
{
	LogInfo("");

	setRenderingApi((VulkanApi)createOptions.api);
	devices = api->getApiDevices();

	LogVerbose("end.")
}


void VulkanRenderingPipeline::init()
{
	LogInfo("");

	// create swapchain
	swapchain = new VulkanSwapChain(VulkanSwapChain::CreateOptions(
		devices,
		nullptr
	));
	swapchain->init();


	// create viewport & scissors
	auto swapDims = swapchain->getDimensions();
	setSwapChain((VulkanSwapChain*)swapchain);

	addViewport(swapDims, float2(0.0f, 1.0f));
	addScissor(swapDims);
	createViewports();

	renderPass = new VulkanSimpleRenderPass(devices);


	if (vkCreatePipelineLayout(devices.vkDevice, &createOptions.pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	createOptions.pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	createOptions.pipelineInfo.stageCount = createOptions.shader->size();
	createOptions.pipelineInfo.pStages = createOptions.shader->getStageInfo();

	createOptions.pipelineInfo.pVertexInputState = &createOptions.vertexInputInfo;
	createOptions.pipelineInfo.pInputAssemblyState = &createOptions.inputAssembly;
	createOptions.pipelineInfo.pViewportState = &createOptions.viewportState;
	createOptions.pipelineInfo.pRasterizationState = &createOptions.rasterizer;
	createOptions.pipelineInfo.pMultisampleState = &createOptions.multisampling;
	createOptions.pipelineInfo.pDepthStencilState = nullptr; // Optional
	createOptions.pipelineInfo.pColorBlendState = &createOptions.colorBlending;
	createOptions.pipelineInfo.pDynamicState = nullptr; // Optional

	createOptions.pipelineInfo.layout = pipelineLayout;

	createOptions.pipelineInfo.renderPass = renderPass->getVkRenderPass();
	createOptions.pipelineInfo.subpass = 0;

	if (vkCreateGraphicsPipelines(devices.vkDevice, VK_NULL_HANDLE, 1, &createOptions.pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	// create framebuffers
	auto swapChainImageViews = swapchain->getImageViews();
	iRect dims = swapchain->getDimensions();
	size_t framebufferCount = swapchain->getSize();

	framebuffers.resize(framebufferCount);

	for (size_t i = 0; i < framebufferCount; i++) {

		VkImageView attachments[] = {
			swapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass->getVkRenderPass();
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = dims.width;
		framebufferInfo.height = dims.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(devices.vkDevice, &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}

	commandPool = new VulkanCommandPool(devices, this, getFramebuffers().size());

	// create semaphores
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.resize(framebufferCount, VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(devices.vkDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(devices.vkDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(devices.vkDevice, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}

	LogVerbose("end.")
}


void VulkanRenderingPipeline::quit()
{
	LogInfo("");

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(devices.vkDevice, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(devices.vkDevice, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(devices.vkDevice, inFlightFences[i], nullptr);
	}

	commandPool->quit();


	for (auto framebuffer : framebuffers) {
		vkDestroyFramebuffer(devices.vkDevice, framebuffer, nullptr);
	}

	vkDestroyPipelineLayout(devices.vkDevice, pipelineLayout, nullptr);

	vkDestroyPipeline(devices.vkDevice, pipeline, nullptr);

	if (renderPass != nullptr)
	{
		renderPass->quit();
	}



	swapchain->quit();

	LogVerbose("end.")
}


VulkanRenderingPipeline::~VulkanRenderingPipeline()
{
	delete commandPool;
	delete swapchain;
	for (auto v : viewports)
	{
		delete v;
	}
	for (auto s : scissors)
	{
		delete s;
	}
	viewports.clear();
	scissors.clear();

	delete renderPass;
}



void VulkanRenderingPipeline::update()
{

}


void VulkanRenderingPipeline::render()
{

	auto device = devices.vkDevice;
	
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(device, swapchain->getVkSwapchain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	auto buf = commandPool->getBuffer(imageIndex);
	submitInfo.pCommandBuffers = &buf;

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	if (vkQueueSubmit(devices.vulkanDevice->getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapchain->getVkSwapchain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(devices.vulkanDevice->getPresentQueue(), &presentInfo);

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanRenderingPipeline::resize(uint32_t width, uint32_t height)
{

}

void VulkanRenderingPipeline::setSwapChain(SwapChain* swapchain_)
{
	LogInfo("");
	swapchain = (VulkanSwapChain*)swapchain_;
}

void VulkanRenderingPipeline::setRenderingApi(RenderingApi* api_)
{
	LogInfo("");
	api = (VulkanApi)api_;
}

void VulkanRenderingPipeline::createViewports()
{
	LogInfo("");
	vkViewports.clear();
	vkScissors.clear();

	for (auto scissor : scissors) {
		vkScissors.push_back(scissorToVkRect(static_cast<VulkanScissor*>(scissor)));
	}


	for (auto view : viewports)
	{
		vkViewports.push_back(viewportToVkViewport(dynamic_cast<VulkanViewport*>(view)));
	}

	createOptions.viewportState.viewportCount = (int)vkViewports.size();
	createOptions.viewportState.scissorCount = (int)vkScissors.size();
	createOptions.viewportState.pViewports = vkViewports.data();
	createOptions.viewportState.pScissors = vkScissors.data();

}

void* VulkanRenderingPipeline::getSwapChain()
{
	LogInfo("");
	return (VulkanSwapChain*)swapchain;
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

