#pragma once
#include "Core/Rendering/SwapChain.h"
#include "Core/Vulkan/VulkanDevice.h"
#include "Core/Vulkan/VulkanApi.h"


class VulkanSwapChain: public SwapChain
{
public:
	VulkanSwapChain(VulkanApi *api);
	~VulkanSwapChain() override
	{
	
	}

	iRect getDimensions() override
	{
		Rect<int> dim(0, 0, (int)swapChainExtent.width, (int)swapChainExtent.height);
		return dim;
	}

	void init() override;
	void quit() override;
	
	void querySwapChainSupport() override
	{
		auto dev = device->getPhyiscalDevice();
		//auto vkphysdev = vkdev->getPhysicalDevice();
		
		swapChainSupport = device->querySwapChainSupport(dev);
	}

	QueueFamilyIndices findQueueFamilies()
	{
		return device->findQueueFamilies(vkPhysDevice);
	}
	
	void createBackBuffers() override;
	void resize(uint32_t width, uint32_t height) override;

	
	VkExtent2D				chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkPresentModeKHR		chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkSurfaceFormatKHR		chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

private:
	VulkanDevice				*device;
	VkPhysicalDevice			vkPhysDevice;
	SwapChainSupportDetails		swapChainSupport{};
	VkSwapchainKHR				swapChain;
	VkExtent2D					swapChainExtent;
	VkFormat					swapChainImageFormat;
	std::vector<VkImage>		swapChainImages;
	std::vector<VkImageView>	swapChainImageViews;
	
};
