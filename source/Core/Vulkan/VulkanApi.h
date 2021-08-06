#pragma once
#include "Core/Rendering/RenderingApi.h"
#include "VulkanDevice.h"

class VulkanApi : public RenderingApi
{
public:
	void	init() override;
	void	quit() override;
	void*	getSwapChain() override { return swapChain; };

	static VulkanApi* create(Window* window, const Device::Options& opts);

	VkInstance				getInstance()		{ return instance; }
	VkSurfaceKHR			getSurface()		{ return surface; }
	VulkanDevice*			getVulkanDevice()	{ return dynamic_cast<VulkanDevice*>(device); }
	VkDevice				getVkDevice()		{ return static_cast<VkDevice>(getVulkanDevice()->getApiDevice()); }
	VkPhysicalDevice		getPhysicalDevice() { return getVulkanDevice()->getPhyiscalDevice(); }

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
