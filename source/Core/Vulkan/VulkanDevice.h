#pragma once

#include "Core/Window/WindowBase.h"
#include "Utils/Debug/Debug.h"


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
		const Options& opts = Options())
	{
		return new VulkanDevice(window, opts);
	}

	void init() override;
	void quit() override;

	~VulkanDevice() override { LogInfo(""); }

protected:
	VkInstance					instance;

	VkDevice					device;
	VkSurfaceKHR				surface;
	VkDebugUtilsMessengerEXT	debugMessenger;
	VkPhysicalDevice			physicalDevice = VK_NULL_HANDLE;

	WindowBase					*window;

private:
	VulkanDevice(WindowBase* window, const Options& opts);

	void createInstance();
	void createSurface();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSwapChain();
	void createImageViews();
	void createGraphicsPipeline();


	std::vector<const char*> getRequiredExtensions();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	bool checkValidationLayerSupport();
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;

#ifndef NDEBUG
	void setupDebugMessenger();
#endif
 
};