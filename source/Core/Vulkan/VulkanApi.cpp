#include "pch.h"
#include "Core/Vulkan/VulkanApi.h"

using namespace std;

VulkanApi* VulkanApi::create(Window *window, const Device::Options &opts)
{
	LogInfo("");
	if(window == nullptr) {
		LogWarning("Window is nullptr");
		return nullptr;
	}
	
	return new VulkanApi(window, opts);
}


VulkanApi::VulkanApi(Window *window, const Device::Options &opts):
	RenderingApi("Vulkan", window, opts)
{
}

void VulkanApi::init()
{
	LogInfo("");
	setDevice(VulkanDevice::create(window, deviceOptions));
	device->init();
	LogDebug("end.");
}

void VulkanApi::quit()
{
	LogInfo("");
	if(device != nullptr) {
		device->quit();
	}
	delete device;
	LogDebug("end.");
}
