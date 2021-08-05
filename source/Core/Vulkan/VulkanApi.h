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
	VkPhysicalDevice_T*		getPhysicalDevice()
	{
		auto apidev = ((VulkanDevice*)device);
		return (VkPhysicalDevice_T*)apidev->getPhyiscalDevice();
	}
	QueueFamilyIndices findQueueFamilies();

protected:
	VulkanApi(Window* window, const Device::Options& opts);
	
	VkInstance		instance;
	VkSurfaceKHR	surface;
	
	void createInstance();
	void createWindowSurface();
	void createSwapChain();

	std::vector<const char*> getRequiredExtensions(); 

	///  debugger
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	bool checkValidationLayerSupport();
	void setupDebugMessenger();
	VkDebugUtilsMessengerEXT debugMessenger;
};
