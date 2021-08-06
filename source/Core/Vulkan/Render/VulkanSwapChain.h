#pragma once
#include "Core/Rendering/SwapChain.h"
#include "Core/Vulkan/VulkanDevice.h"
#include "Core/Vulkan/VulkanApi.h"


class VulkanSwapChain : public SwapChain
{
public:
	// pack all you need here
	class CreateOptions
	{
	public:
		VulkanDevices		devices;
		VulkanSwapChain*	oldSwapChain	= nullptr;

		CreateOptions(const VulkanDevices& devices, VulkanSwapChain* oldSwapChain = nullptr):
			devices(devices), oldSwapChain(oldSwapChain)
		{
			
		}

		CreateOptions()
		{}
	};
	
	VulkanSwapChain(CreateOptions options);
	~VulkanSwapChain() = default;
		
	void init() override;
	void quit() override;
	void createFrameBuffers() override;
	void resize(uint32_t width, uint32_t height) override;

	iRect getDimensions() override
	{
		Rect<int> dim(0, 0, (int)extent.width, (int)extent.height);
		return dim;
	}

	void					querySwapChainSupport() override {	swapChainSupport = _getVulkanDevice()->querySwapChainSupport(_getVkPhyicalDevice()); }
	QueueFamilyIndices		findQueueFamilies() { return _getVulkanDevice()->findQueueFamilies(_getVkPhyicalDevice()); }


	VulkanApi				_getVulkanApi() { return devices.vulkanApi; }
	VulkanDevice*			_getVulkanDevice() { return devices.vulkanDevice; }
	VkDevice				_getVkDevice() { return devices.vkDevice; }
	VkPhysicalDevice		_getVkPhyicalDevice() {	return devices.vkPhysicalDevice; }
	
	VkExtent2D				chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkPresentModeKHR		chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkSurfaceFormatKHR		chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	
private:
	VulkanDevices					devices;
	CreateOptions					createOptions;
	VkSwapchainCreateInfoKHR		createInfo{};
	SwapChainSupportDetails			swapChainSupport{};

	VkSurfaceFormatKHR				surfaceFormat;
	VkPresentModeKHR				presentMode;
	VkExtent2D						extent;

	uint32_t						backBufferCount;
	VkExtent2D						swapChainExtent;
	VkFormat						swapChainImageFormat;

	
	VkSwapchainKHR					swapChain;
	std::vector<VkImage>			swapChainImages;
	std::vector<VulkanImageView*>	swapChainImageViews;

};
