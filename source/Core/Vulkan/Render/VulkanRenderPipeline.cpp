#include "pch.h"

#include <glm/ext/matrix_transform.hpp>

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

	LogInfo("Renderpass");
	renderPass = new VulkanSimpleRenderPass(devices);


	createDescriptorSetLayout();

	createVertexInput();
	
	createPipeline();
	

	createFrameBuffers();

	createCommandPool();
	
	texture = new VulkanTexture(devices, "Data/Textures/texture.jpg");
	texture->addToCommandPool(commandPool);
	
	textureSampler = new VulkanSampler(devices, texture);
	textureSampler->load();

	
	
	createUniformBuffers();


	createDescriptorPool();
	createDescriptorSets();
	
	commandPool->createBuffers();
	
	createSemaphores();

	LogVerbose("end.")
}


void VulkanRenderingPipeline::quit()
{
	LogInfo("");

	swapchain->quit();

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(devices.vkDevice, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(devices.vkDevice, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(devices.vkDevice, inFlightFences[i], nullptr);
	}

	delete texture;

	commandPool->quit();


	for (auto framebuffer : framebuffers) {
		vkDestroyFramebuffer(devices.vkDevice, framebuffer, nullptr);
	}

	vkDestroyPipelineLayout(devices.vkDevice, pipelineLayout, nullptr);

	vkDestroyPipeline(devices.vkDevice, pipeline, nullptr);

	delete uniformBuffer;


	vkDestroyDescriptorPool(devices.vkDevice, descriptorPool, nullptr);

	if (renderPass != nullptr)
	{
		renderPass->quit();
	}

	vkDestroyDescriptorSetLayout(devices.vkDevice, descriptorSetLayout, nullptr);
	LogVerbose("end.")
}


void VulkanRenderingPipeline::createPipeline()
{
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

	if (vkCreatePipelineLayout(devices.vkDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	createOptions.pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	createOptions.pipelineInfo.stageCount = createOptions.shader->size();
	createOptions.pipelineInfo.pStages = createOptions.shader->getStageInfo();
	createOptions.pipelineInfo.pVertexInputState = &vertexInputInfo;
	createOptions.pipelineInfo.pInputAssemblyState = &createOptions.inputAssembly;
	createOptions.pipelineInfo.pViewportState = &createOptions.viewportState;
	createOptions.pipelineInfo.pRasterizationState = &createOptions.rasterizer;
	createOptions.pipelineInfo.pMultisampleState = &createOptions.multisampling;
	createOptions.pipelineInfo.pColorBlendState = &createOptions.colorBlending;
	createOptions.pipelineInfo.layout = pipelineLayout;
	createOptions.pipelineInfo.renderPass = renderPass->getVkRenderPass();
	createOptions.pipelineInfo.subpass = 0;
	createOptions.pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;


	if (vkCreateGraphicsPipelines(devices.vkDevice, VK_NULL_HANDLE, 1, &createOptions.pipelineInfo, nullptr, &pipeline)
		!= VK_SUCCESS)
	{
		throw std::runtime_error("failed to create graphics pipeline!");
	}
}


void VulkanRenderingPipeline::createDescriptorSetLayout()
{
	LogInfo("");
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.pImmutableSamplers = nullptr;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(devices.vkDevice, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}


void VulkanRenderingPipeline::createDescriptorPool()
{
	std::array<VkDescriptorPoolSize, 2> poolSizes{};
	auto swapChainImages = swapchain->getSwapChainImages();

	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(swapChainImages.size());
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(swapChainImages.size());

	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());

	if (vkCreateDescriptorPool(devices.vkDevice, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}


void VulkanRenderingPipeline::createDescriptorSets()
{
	auto swapChainImages = swapchain->getSwapChainImages();

	LogInfo(std::to_string(swapChainImages.size()));

	layouts = std::vector<VkDescriptorSetLayout>(swapChainImages.size(), descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(swapChainImages.size());
	if (vkAllocateDescriptorSets(devices.vkDevice, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	auto uniformBuffers = uniformBuffer->getBuffers();

	for (size_t i = 0; i < swapChainImages.size(); i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(mvpUniform);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureSampler->getTextureImageView();
		imageInfo.sampler = textureSampler->getTextureSampler();

		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(devices.vkDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}


void VulkanRenderingPipeline::createVertexInput()
{
	LogInfo("");
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	bindingDescription = Vertex3::getBindingDescription();
	attributeDescriptions = Vertex3::getAttributeDescriptions();

	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
}


void VulkanRenderingPipeline::createUniformBuffers()
{
	LogInfo("");

	float width = swapchain->getDimensions().width;
	float height = swapchain->getDimensions().height;

	uniforms.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	uniforms.view = glm::lookAt(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	uniforms.proj = glm::perspective(glm::radians(90.0f), width / height, 0.01f, 10000.0f);
	uniforms.proj[1][1] *= -1;

	uniformBuffer = new VulkanUniformBuffer(devices);
	uniformBuffer->create(uniforms);
}


void VulkanRenderingPipeline::createFrameBuffers()
{
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
}


void VulkanRenderingPipeline::createCommandPool()
{
	LogInfo("CommandPool");
	commandPool = new VulkanCommandPool(devices, this, getFramebuffers().size());
}


void VulkanRenderingPipeline::createSemaphores()
{
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.resize(swapchain->getSize(), VK_NULL_HANDLE);

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
}

VulkanRenderingPipeline::~VulkanRenderingPipeline()
{
	LogInfo("");
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
	auto time = (float)Clock::getElapsedTime();

	uniforms.model = glm::rotate(glm::mat4(1.0f), (time * 0.0001f) * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	uniformBuffer->update(commandPool, imageIndex, uniforms);
}


void VulkanRenderingPipeline::render()
{

	auto device = devices.vkDevice;

	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);


	vkAcquireNextImageKHR(device, swapchain->getVkSwapchain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	update();
	
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

	auto result = vkQueuePresentKHR(devices.vulkanDevice->getPresentQueue(), &presentInfo);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR /*|| framebufferResized*/) {
		//	framebufferResized = false;
			//recreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}
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

