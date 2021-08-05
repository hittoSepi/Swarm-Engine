#pragma once
#include "Core/Rendering/SwapChain.h"
#include "Core/Vulkan/VulkanDevice.h"
#include "Core/Vulkan/VulkanApi.h"


class VulkanSwapChain : public SwapChain
{
public:
	VulkanSwapChain(VulkanApi* api);
	~VulkanSwapChain() override
	{

	}
	void init() override;
	void quit() override;

	iRect getDimensions() override
	{
		Rect<int> dim(0, 0, (int)swapChainExtent.width, (int)swapChainExtent.height);
		return dim;
	}

	void querySwapChainSupport() override
	{
		auto dev = device->getPhyiscalDevice();
		swapChainSupport = device->querySwapChainSupport(dev);
	}

	QueueFamilyIndices findQueueFamilies()
	{
		return device->findQueueFamilies(vkPhysDevice);
	}

	void createBackBuffers() override;
	void resize(uint32_t width, uint32_t height) override;

	VulkanApi*				_getVulkanApi() { return dynamic_cast<VulkanApi*>(renderingApi); }
	VulkanDevice*			_getVulkanDevice() { return device; }
	VkDevice				_getVkDevice() { return static_cast<VkDevice>(device->getApiDevice()); }
	VkPhysicalDevice		_getVkPhyicalDevice()
	{
		auto dev = _getVulkanDevice();
		return dev->getPhyiscalDevice();
	}
	
	VkExtent2D				chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkPresentModeKHR		chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkSurfaceFormatKHR		chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);




private:
	VulkanDevice* device;
	VkPhysicalDevice			vkPhysDevice;
	SwapChainSupportDetails		swapChainSupport{};
	VkSwapchainKHR				swapChain;
	VkExtent2D					swapChainExtent;
	VkFormat					swapChainImageFormat;
	std::vector<VkImage>		swapChainImages;
	std::vector<VkImageView>	swapChainImageViews;

};
