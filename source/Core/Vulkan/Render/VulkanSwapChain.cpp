#include "pch.h"
#include "VulkanSwapChain.h"
#include "Core/Rendering/SwapChain.h"


VulkanSwapChain::VulkanSwapChain(CreateOptions options):
	createOptions(options), devices(options.devices), SwapChain()
{
	LogInfo("");
}


void VulkanSwapChain::quit()
{
	LogInfo("")
	auto dev = devices.vkDevice;
	for(uint32_t i = 0; i <= backBufferCount-1; i++)
	{
		delete swapChainImageViews[i];
	}
	swapChainImageViews.clear();
	vkDestroySwapchainKHR(dev, swapChain, nullptr);
}


void VulkanSwapChain::init()
{
	LogInfo("");
	
	
	querySwapChainSupport();
	
	surfaceFormat		= chooseSwapSurfaceFormat(swapChainSupport.formats);
	presentMode			= chooseSwapPresentMode(swapChainSupport.presentModes);
	extent				= chooseSwapExtent(swapChainSupport.capabilities);

	
	backBufferCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && backBufferCount > swapChainSupport.capabilities.maxImageCount) {
		backBufferCount = swapChainSupport.capabilities.maxImageCount;
	}

	createInfo.sType				= vk_ci::SW_VULKAN_SWAPCHAIN;
	createInfo.surface				= devices.surface;
	createInfo.minImageCount		= backBufferCount;
	createInfo.imageFormat			= surfaceFormat.format;
	createInfo.imageColorSpace		= surfaceFormat.colorSpace;
	createInfo.imageExtent			= extent;
	createInfo.imageArrayLayers		= 1;
	createInfo.imageUsage			= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	
	QueueFamilyIndices indices		= findQueueFamilies();
	uint32_t queueFamilyIndices[]	= { indices.graphicsFamily.value(), indices.presentFamily.value() };

	
	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode			= VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount	= 2;
		createInfo.pQueueFamilyIndices		= queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode			= VK_SHARING_MODE_EXCLUSIVE;
	}

	
	createInfo.preTransform		= swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha	= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode		= presentMode;
	createInfo.clipped			= VK_TRUE;
	createInfo.oldSwapchain		= VK_NULL_HANDLE;

	
	if (vkCreateSwapchainKHR(_getVkDevice(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
		LogError(std::string("failed to create swap chain!"));
	}
	
	createFrameBuffers();
}


VkExtent2D VulkanSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	LogInfo("");


	if (capabilities.currentExtent.width != UINT32_MAX) {
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		auto win = devices.window;
		glfwGetFramebufferSize((GLFWwindow*)win->getNativeWindow(), &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

VkPresentModeKHR VulkanSwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	LogInfo("");


	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	LogInfo("");
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT::RGBA8 && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}
	return availableFormats[0];
}


void VulkanSwapChain::createFrameBuffers() {

	LogInfo(" Creating SwapchainImages and Imageview.");
	vkGetSwapchainImagesKHR(devices.vkDevice, swapChain, &backBufferCount, nullptr);
	
	swapChainImages.resize(backBufferCount);
	
	vkGetSwapchainImagesKHR(devices.vkDevice, swapChain, &backBufferCount, swapChainImages.data());

	swapChainImageFormat	= surfaceFormat.format;
	swapChainExtent			= extent;

	swapChainImageViews.resize(swapChainImages.size());
	
	for (int i = 0; i < swapChainImages.size(); i++) 
	{
		swapChainImageViews[i] = new VulkanImageView(VulkanImageView::CreateOptions( 
			devices.vkDevice,
			swapChainImages[i],
			swapChainImageFormat,
			VK_IMG::TEX2D
		));
	}
}

void VulkanSwapChain::resize(uint32_t width, uint32_t height)
{
	
}