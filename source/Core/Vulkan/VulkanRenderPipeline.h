#pragma once

class VulkanRenderingPipeline: public RenderingPipeline
{
public:
	VulkanRenderingPipeline(VulkanDevice *device);
	virtual void init() override;
	virtual void quit() override;
	virtual void update() override;
	virtual void render() override;
	virtual void resize(uint32_t width, uint32_t height) override;
private:
	VulkanShader *testshader;
};