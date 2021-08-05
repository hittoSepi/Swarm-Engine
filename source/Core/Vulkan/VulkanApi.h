#pragma once
#include "Core/Rendering/RenderingApi.h"
#include "VulkanDevice.h"

class VulkanApi : public RenderingApi
{
public:
	void init() override;
	void quit() override;
	void* getSwapChain() override { return swapChain; };

	static VulkanApi* create(Window* window, const Device::Options& opts);

	VkSurfaceKHR&			getSurface()		{ return surface; }
	VkInstance&				getInstance()		{ return instance; }
	VkPhysicalDevice		getPhysicalDevice()
	{
		auto apidev = ((VulkanDevice*)device);
		return (VkPhysicalDevice)apidev->getPhyiscalDevice();
	}
	

protected:
	VulkanApi(Window* window, const Device::Options& opts);
	
	VkInstance		instance;
	VkSurfaceKHR	surface;
	
	void createInstance();
	void createWindowSurface();

	
	std::vector<const char*> getRequiredExtensions(); 

	///  debugger
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	bool checkValidationLayerSupport();
	void setupDebugMessenger();
	VkDebugUtilsMessengerEXT debugMessenger;

	/*
	Device				*device;
	WindowBase			*window;
	Device::Options		deviceOptions;
	const std::string	apiName;

	RenderingPipeline	*renderingPipeline;
	SwapChain			*swapChain;
	*/
};
