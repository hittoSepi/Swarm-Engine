#pragma once

inline const int MAX_VIEWPORT = 10;

class Scissor : public iRect
{
public:
	Scissor() :
		iRect(0, 0, 0, 0)
	{
	}

	Scissor(const iRect& scissorsArea_) :
		iRect(scissorsArea_)
	{
	}
};


/// Render Pipeline abstraction
class RenderingPipeline
{
public:
	virtual void init() = 0;
	virtual void quit() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void resize(uint32_t width, uint32_t height) = 0;
	virtual void setRenderingApi(RenderingApi* api) = 0;
	virtual void setSwapChain(SwapChain* swapchain) = 0;

	virtual void addViewport(const Rect<int>& view_area, const float2& depth) = 0;
	virtual void addViewport(Viewport* viewport) = 0;

	virtual void addScissor(Scissor* scissor) = 0;
	virtual void addScissor(const iRect& view_area) = 0;

	virtual void* getSwapChain() { return swapChain; }
	//virtual void* getViewport() { return viewport; }
protected:

	std::string getName() { return debugName; }
	RenderingPipeline(const std::string& debugName_) :
		debugName(debugName_)
	{}

	//std::vector<Viewport*> viewports;
	std::vector<Viewport*> viewports;
	std::vector<Scissor*> scissors;
		

	RenderingApi* api;
	Device* device;
	SwapChain* swapChain;
	Viewport* viewport;
private:
	std::string debugName;
};
