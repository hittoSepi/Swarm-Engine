#pragma once

#include "Core/Window/WindowBase.h"


class _VulkanApi;

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};


struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};



class VulkanDevice : public Device
{
public:
	inline static VulkanDevice* create(
		WindowBase* window,
		VkInstance& instance,
		VkSurfaceKHR& surface,
		const Options& opts = Options())
	{
		return new VulkanDevice(window, instance, surface, opts);
	}

	void init() override;
	void quit() override;

	void* getApiDevice() override { return device; }
	VkPhysicalDevice getPhysicalDevice() { return physicalDevice; }

	~VulkanDevice() override;

	QueueFamilyIndices		findQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

protected:
	VkDevice					device;
	VkPhysicalDevice			physicalDevice = VK_NULL_HANDLE;
	WindowBase					*window;

private:
	VulkanDevice(_VulkanApi api, const Options& opts);
	VulkanDevice(WindowBase* window, VkInstance& instance, VkSurfaceKHR &sufrace, const Options& opts);

	void createInstance();
	void createSurface();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createImageViews();
	void createGraphicsPipeline();

	std::vector<const char*> getRequiredExtensions();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	bool checkValidationLayerSupport();
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	
	_VulkanApi *api;
				
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	
	VkInstance&		instance;
	VkSurfaceKHR&	surface;
	
#ifndef NDEBUG
	void setupDebugMessenger();
#endif
 
};