#pragma once
#include <string>
#include "stdexcept"

#include "Core/Window/WindowBase.h"
#include "Core/Device.h"
#include "Core/Rendering/SwapChain.h"
#include "Core/Rendering/RenderingPipeline.h"


/// Rendering api abstraction
class RenderingApi
{
public:
	virtual ~RenderingApi()
	{
		delete renderingPipeline;
		delete swapChain;
	
	}
	
	virtual void init() = 0;
	virtual void quit() = 0;
	
	virtual WindowBase*		getWindow()				{ return window; }
	virtual Device::Options getDeviceOptions()		{ return deviceOptions; }
	const std::string&		getApiName()			{ return apiName; }
	virtual void*			getSwapChain()			= 0;

	void setDevice(Device* dev)
	{
		if(dev == nullptr)
		{
			throw std::runtime_error("Device is null.");
		}
		device = dev;
	}
	Device* getDevice() { return device; }
	void* getApiDevice() { return device->getApiDevice(); }

protected:
	RenderingApi(const std::string &apiname, Window *window, const Device::Options &opts):
		window(window), apiName(apiname), deviceOptions(opts)
	{}

	Device				*device;
	WindowBase			*window;
	Device::Options		deviceOptions;
	const std::string	apiName;

	RenderingPipeline	*renderingPipeline;
	SwapChain			*swapChain;
};
