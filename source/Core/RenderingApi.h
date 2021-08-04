#pragma once
#include <string>

// Rendering api abstaction
class RenderingApi
{
public:
	virtual ~RenderingApi() {}
	
	virtual void init() = 0;
	virtual void quit() = 0;
	
	virtual WindowBase*		getWindow()					{ return window; }
	virtual Device::Options getDeviceOptions()			{ return deviceOptions; }
	
	const std::string& getApiName() { return apiName; }

	void setDevice(Device* dev)
	{
		if(dev == nullptr)
		{
			throw std::runtime_error("Device is null.");
		}
		device = dev;
	}
	Device* getDevice() { return device; }
protected:
	RenderingApi(const std::string &apiname, Window *window, const Device::Options &opts):
		window(window), apiName(apiname), deviceOptions(opts)
	{}

	Device				*device;
	WindowBase			*window;
	Device::Options		deviceOptions;
	const std::string	apiName;
};
