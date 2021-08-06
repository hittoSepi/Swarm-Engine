#pragma once
class RenderingApi;

class SwapChain
{
public:
	SwapChain()
	{
	}
	virtual ~SwapChain() {}
	
	virtual void init() = 0;
	virtual void quit() = 0;
	
	virtual void querySwapChainSupport() = 0;
	virtual void createFrameBuffers() = 0;
	virtual void resize(uint32_t width, uint32_t height) = 0;

	virtual iRect getDimensions() = 0;
protected:
	
};
