#pragma once
class RenderingApi;

class SwapChain
{
public:
	SwapChain(RenderingApi *api):
		renderingApi(api)
	{
	}
	
	virtual void init() = 0;
	virtual void querySwapChainSupport() = 0;
	virtual void createBackBuffers() = 0;
	virtual void resize(uint32_t width, uint32_t height) = 0;

	
protected:
	RenderingApi* renderingApi;
};
