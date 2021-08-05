#pragma once


/// Render Pipeline abstraction
class RenderingPipeline
{
public:
	virtual void init() = 0;
	virtual void quit() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void resize(uint32_t width, uint32_t height) = 0;

	std::string getName() { return debugName; }
protected:
	RenderingPipeline(const std::string &debugName_, Device* device_):
		device(device_), debugName(debugName_)
	{}

	Device* device;
private:
	std::string debugName;
};
