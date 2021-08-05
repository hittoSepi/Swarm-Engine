#pragma once
#include "Core/Rendering/SwapChain.h"

class VulkanSwapChain: public SwapChain
{
public:
	VulkanSwapChain(VulkanApi *api);
	~VulkanSwapChain()
	{
		auto dev = (VulkanDevice*)renderingApi->getDevice();
		const VkDevice &apidev = (VkDevice)dev->getApiDevice();
		
		for (auto imageView : swapChainImageViews) {
			vkDestroyImageView(apidev, imageView, nullptr);
		}
		vkDestroySwapchainKHR(apidev, swapChain, nullptr);
	}

	void init() override;
	void querySwapChainSupport() override
	{
		swapChainSupport = querySwapChainSupportImnpl();
	}
	void createBackBuffers() override;
	void resize(uint32_t width, uint32_t height) override;

	SwapChainSupportDetails querySwapChainSupportImnpl();
	VkExtent2D				chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkPresentModeKHR		chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkSurfaceFormatKHR		chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

private:
	SwapChainSupportDetails		swapChainSupport;
	VulkanDevice				*device;
	VkSwapchainKHR				swapChain;
	VkExtent2D					swapChainExtent;
	VkFormat					swapChainImageFormat;
	std::vector<VkImage>		swapChainImages;
	std::vector<VkImageView>	swapChainImageViews;
	
};
