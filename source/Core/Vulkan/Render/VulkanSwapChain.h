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
		VulkanApi*			api				= nullptr;
		VkSurfaceKHR		surface			= nullptr;
		VulkanDevice*		device			= nullptr;
		VkDevice			vkDevice		= nullptr;
		VkPhysicalDevice	vkPhysDevice	= nullptr;
		VulkanSwapChain*	oldSwapChain	= nullptr;

		CreateOptions(
			VulkanApi*			api,
			VkSurfaceKHR		surface,
			VulkanDevice*		device,		
			VkDevice			vkDevice,	
			VkPhysicalDevice	vkPhysDevice,
			VulkanSwapChain*	oldSwapChain = nullptr):
			api(api),
			surface(surface), device(device), vkDevice(vkDevice),
			vkPhysDevice(vkPhysDevice), oldSwapChain(oldSwapChain)
		{
			
		}

		CreateOptions()
		{}
	};
	
	VulkanSwapChain(CreateOptions options);
	VulkanSwapChain(VkSurfaceKHR surface,  VulkanApi* rendering_api);
	
	~VulkanSwapChain() override
	{
	}
		
	void init() override;
	void quit() override;
	void createFrameBuffers() override;
	void resize(uint32_t width, uint32_t height) override;

	iRect getDimensions() override
	{
		Rect<int> dim(0, 0, (int)extent.width, (int)extent.height);
		return dim;
	}

	QueueFamilyIndices		findQueueFamilies() { return _getVulkanDevice()->findQueueFamilies(_getVkPhyicalDevice()); }
	void					querySwapChainSupport() override {	swapChainSupport = _getVulkanDevice()->querySwapChainSupport(_getVkPhyicalDevice()); }


	VulkanApi*				_getVulkanApi() { return createOptions.api; }
	VulkanDevice*			_getVulkanDevice() { return createOptions.device; }
	VkDevice				_getVkDevice() { return createOptions.vkDevice; }
	VkPhysicalDevice		_getVkPhyicalDevice() {	return createOptions.vkPhysDevice; }
	
	VkExtent2D				chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkPresentModeKHR		chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkSurfaceFormatKHR		chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	
private:
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
