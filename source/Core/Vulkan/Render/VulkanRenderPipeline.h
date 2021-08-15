#pragma once
#include "Swarm.h"

class VulkanRenderPass;

class VulkanRenderingPipeline : public RenderingPipeline
{
public:
	class CreateOptions
	{
	public:
		VulkanApi			api = nullptr;

		CreateOptions() {}
		CreateOptions(VulkanApi	api) : api(api) {}



		void setViewPorts(std::vector<VulkanViewport> vps)
		{
			viewportState.viewportCount = (uint32_t)vps.size();
			viewportState.pViewports = vps.data();
		}

		void setScissors(std::vector<VulkanScissor> scs)
		{
			viewportState.scissorCount = (uint32_t)scs.size();
			viewportState.pScissors = scs.data();
		}


		struct Flags
		{
		};

		VkPipelineLayoutCreateInfo pipelineLayoutInfo
		{
			VK_CI::SW_VULKAN_PIPELINE_LAYOUT,
			nullptr,
			0,
			0,
			nullptr
		};

		// Dynamic states
		std::vector<VkDynamicState>	dynamicStates{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		VkPipelineDynamicStateCreateInfo dynamicState
		{
			VK_CI::SW_VULKAN_PIPELINE_DYNAMIC,
			nullptr,
			0,
			(uint32_t)dynamicStates.size(),
			dynamicStates.data()
		};

		// Input Assembly
		VkPipelineInputAssemblyStateCreateInfo inputAssembly
		{
			VK_CI::SW_VULKAN_PIPELINE_INPUT_ASSEMBLY,
			nullptr,
			0,
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			 VK_FALSE,
		};


		// Viewport
		VkPipelineViewportStateCreateInfo viewportState
		{
			VK_CI::SW_VULKAN_PIPELINE_VIEWPORT,
			nullptr,
			0
		};

		// Rasterizer
		VkPipelineRasterizationStateCreateInfo rasterizer
		{
			VK_CI::SW_VULKAN_PIPELINE_RASTERIZATION,
			nullptr,
			0,
			VK_FALSE,
			VK_FALSE,
			VK_POLYGON_MODE_FILL,
			VK_CULL_MODE_BACK_BIT,
			VK_FRONT_FACE_COUNTER_CLOCKWISE,
			 VK_FALSE,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		// Multisampling
		VkPipelineMultisampleStateCreateInfo multisampling
		{
			VK_CI::SW_VULKAN_PIPELINE_MULTISAMPLE,
			nullptr,
			0,
			VK_SAMPLE_COUNT_1_BIT,
			VK_FALSE
		};

		// Color blending		
		VkPipelineColorBlendAttachmentState colorBlendAttachment
		{
			VK_FALSE,
			VkBlendFactor::VK_BLEND_FACTOR_SRC_COLOR,
			VkBlendFactor::VK_BLEND_FACTOR_DST_COLOR,
			VkBlendOp::VK_BLEND_OP_ADD,
			VkBlendFactor::VK_BLEND_FACTOR_SRC_ALPHA,
			VkBlendFactor::VK_BLEND_FACTOR_DST_ALPHA,
			VkBlendOp::VK_BLEND_OP_MIN,
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
		};


		VkPipelineColorBlendStateCreateInfo colorBlending
		{
			VK_CI::SW_VULKAN_PIPELINE_COLOR_BLEND,
			nullptr,
			0,
			VK_FALSE,
			VK_LOGIC_OP_COPY,
			1,
			&colorBlendAttachment,
			0.0f,
			0.0f,
			0.0f,
			0.0f
		};

		VulkanShader* shader;

		VkGraphicsPipelineCreateInfo pipelineInfo;
	};

	VulkanRenderingPipeline(CreateOptions createOptions);
	~VulkanRenderingPipeline() override;

	virtual void				init() override;
	virtual void				quit() override;
	virtual void				update() override;
	virtual void				render() override;
	virtual void				resize(uint32_t width, uint32_t height) override;
	virtual void				setRenderingApi(RenderingApi* api) override;
	virtual void				setSwapChain(SwapChain* swapchain) override;

	virtual void				addViewport(const iRect& view_area, const float2& depth) override;
	virtual void				addViewport(Viewport* viewport) override;
	virtual void				addScissor(Scissor* scissor) override;
	virtual void				addScissor(const iRect& view_area) override;

	void* getSwapChain();
	VulkanSwapChain* getVulkanSwapChain() { return dynamic_cast<VulkanSwapChain*>(swapchain); }
	VulkanApi getVulkanApi() { return api; }
	VulkanRenderPass* getRenderPass() { return renderPass; }
	std::vector<VkFramebuffer>	getFramebuffers() { return framebuffers; }
	VkPipeline getVkPipeline() { return pipeline; }
	VkPipelineLayout getVkPipelineLayout() { return pipelineLayout; }
	std::vector<VkDescriptorSet> getDescriptorSets() { return descriptorSets; }

	

private:
	
	void						createViewports();
	void						createDescriptorSetLayout();
	void						createDescriptorPool();
	void						createDescriptorSets();
	void						createVertexInput();
	void						createPipeline();
	void						createUniformBuffers();
	void						createFrameBuffers();
	void						createCommandPool();
	void						createSemaphores();


	VkRect2D					scissorToVkRect(VulkanScissor* rect);
	VkViewport					viewportToVkViewport(VulkanViewport* view);
	VkPipeline					pipeline;
	VkPipelineLayout			pipelineLayout;
	std::vector<VkViewport>		vkViewports;
	std::vector<VkRect2D>		vkScissors;

	CreateOptions				createOptions;

	std::vector<Viewport*>		viewports;
	std::vector<Scissor*>		scissors;

	VulkanDevices				devices;
	VulkanApi					api;
	VulkanDevice* device;

	VulkanRenderPass* renderPass;
	VulkanSwapChain* swapchain;
	VulkanCommandPool* commandPool;
	VulkanUniformBuffer* uniformBuffer;
	mvpUniform uniforms;

	VulkanTexture *texture;
	VulkanSampler *textureSampler;
	
	std::vector<VkFramebuffer>				framebuffers;
	VkPipelineVertexInputStateCreateInfo	vertexInputInfo{};

	VkDescriptorSetLayout					descriptorSetLayout;
	VkDescriptorPool						descriptorPool;
	VkDescriptorPoolCreateInfo				poolInfo{};
	VkDescriptorPoolSize					poolSize{};
	std::vector<VkDescriptorSetLayout>		layouts;
	std::vector<VkDescriptorSet>			descriptorSets;

	VkVertexInputBindingDescription bindingDescription;
	std::array<VkVertexInputAttributeDescription, 3>  attributeDescriptions;

	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	VkDescriptorSetLayoutCreateInfo layoutInfo{};

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};


	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;
	uint32_t imageIndex;
	/*
	SwapChain* swapChain;
	Viewport* viewport;
	*/
};
