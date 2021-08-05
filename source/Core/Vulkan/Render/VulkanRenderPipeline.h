#pragma once

class VulkanRenderingPipeline : public RenderingPipeline
{
public:

	VulkanRenderingPipeline(VulkanApi* api);
	
	virtual void init() override;
	virtual void quit() override;
	virtual void update() override;
	virtual void render() override;
	virtual void resize(uint32_t width, uint32_t height) override;
	virtual void setRenderingApi(RenderingApi *api) override;
	virtual void setSwapChain(SwapChain *swapchain) override;
	
	virtual void addViewport(const iRect& view_area, const float2& depth) override;
	virtual void addViewport(Viewport* viewport) override;
	virtual void addScissor(Scissor* scissor) override;
	virtual void addScissor(const iRect& view_area) override;
	
	virtual void* getSwapChain() override;
//	virtual void* getViewport() override;

	VulkanApi* getVulkanApi() { return (VulkanApi*)api; }
private:
	void createViewports();

	VkRect2D	scissorToVkRect(VulkanScissor* rect);
	VkViewport	viewportToVkViewport(VulkanViewport* view);

	std::vector<VkViewport> vkViewports;
	std::vector<VkRect2D> vkScissors;
	
	VkPipelineViewportStateCreateFlags flags{};
	VkPipelineViewportStateCreateInfo viewportState{};
	VkPipelineDynamicStateCreateInfo dynamicState{};

	/*
	std::vector<Viewport*> viewports;
	std::vector<Scissor*> scissors;
		

	RenderingApi* api;
	Device* device;
	SwapChain* swapChain;
	Viewport* viewport;
	*/
};