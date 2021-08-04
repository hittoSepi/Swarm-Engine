#pragma once

#include "Utils/Debug/Debug.h"
#include "Core/RenderingApi.h"

class VulkanApi: public RenderingApi
{
public:
	~VulkanApi()
	{
		LogInfo("");
	}
	
	void init() override;
	void quit() override;
	
	
	static VulkanApi* create(Window *window, const Device::Options &opts);
	
protected:
	VulkanApi(Window *window, const Device::Options &opts);
	

};
