#pragma once

class VulkanRenderingPipeline : public RenderingPipeline
{
public:
	class CreateInfos
	{
	public:
		CreateInfos(std::vector<VkViewport> vps, std::vector<VkRect2D> scs)
		{
			viewportState.viewportCount = (uint32_t)vps.size();
			viewportState.scissorCount = (uint32_t)scs.size();
			viewportState.pViewports = vps.data();
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
	        0
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
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			inputAssembly.primitiveRestartEnable = VK_FALSE,
		};

		// Vertex Input
		VkPipelineVertexInputStateCreateInfo vertexInputInfo
		{
			VK_CI::SW_VULKAN_PIPELINE_VERTEX_INPUT,		// sType
			nullptr,														// pNext
			0,																// flags
			0,																// binding descrioption count
			nullptr,														// binding descrioptions
			0,																// attribute desciption count
			nullptr															// attribute desciptions
		};

		// Viewport
		VkPipelineViewportStateCreateInfo viewportState
		{
			VK_CI::SW_VULKAN_PIPELINE_VIEWPORT,
			nullptr,
			0
		};

		VkPipelineRasterizationStateCreateInfo rasterizer
		{
			VK_CI::SW_VULKAN_PIPELINE_RASTERIZATION,
			nullptr,
			0,
			VK_FALSE,
			VK_FALSE,
			rasterizer.polygonMode = VK_POLYGON_MODE_FILL,
			VK_CULL_MODE_BACK_BIT,
			VK_FRONT_FACE_CLOCKWISE,
			 VK_FALSE,
			1.0f, 1.0f, 1.0f
		};

		VkPipelineMultisampleStateCreateInfo multisampling
		{
			VK_CI::SW_VULKAN_PIPELINE_MULTISAMPLE,
			nullptr,
			0,
			VK_SAMPLE_COUNT_1_BIT,
			VK_FALSE
		};


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
	};


	VulkanRenderingPipeline(VulkanApi* api);

	virtual void	init() override;
	virtual void	quit() override;
	virtual void	update() override;
	virtual void	render() override;
	virtual void	resize(uint32_t width, uint32_t height) override;
	virtual void	setRenderingApi(RenderingApi* api) override;
	virtual void	setSwapChain(SwapChain* swapchain) override;

	virtual void	addViewport(const iRect& view_area, const float2& depth) override;
	virtual void	addViewport(Viewport* viewport) override;
	virtual void	addScissor(Scissor* scissor) override;
	virtual void	addScissor(const iRect& view_area) override;

	virtual void*	getSwapChain() override;

	VulkanApi*		getVulkanApi() { return (VulkanApi*)api; }
private:
	void			createViewports();
	VkRect2D		scissorToVkRect(VulkanScissor* rect);
	VkViewport		viewportToVkViewport(VulkanViewport* view);

	VkPipelineLayout			pipelineLayout;
	std::vector<VkViewport>		vkViewports;
	std::vector<VkRect2D>		vkScissors;
	
	CreateInfos createInfos = CreateInfos(vkViewports, vkScissors);
	/*
	std::vector<Viewport*> viewports;
	std::vector<Scissor*> scissors;


	RenderingApi* api;
	Device* device;
	SwapChain* swapChain;
	Viewport* viewport;
	*/
};