#pragma once

class Device
{
public:
	class Options
	{
	public:
		bool vsync = false;
		#ifdef NDEBUG
		bool enableValidationLayers = false;
		#else
		bool enableValidationLayers = true;
		#endif
	};

	virtual ~Device() {}
	
	virtual void init() = 0;
	virtual void quit() = 0;

protected:
	Device(const Options &opts):
		options(opts)
	{}
	
	Options options;
};