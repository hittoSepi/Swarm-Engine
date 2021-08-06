#pragma once
#include "Core/Rendering/RenderingApi.h"
#include "Core/Vulkan/VulkanDevice.h"

using VulkanSurface		= VkSurfaceKHR; // TODO: proper abstactions
using VulkanInstance	= VkInstance;

using VulkanApi = _VulkanApi*;

class _VulkanApi : public RenderingApi
{
public:
	/// <summary>
	/// Little struct to keep devices and useful stuff in one place 
	/// <br> <br>
	/// probably fails if used before <b> ::updateApiDeviceHelper() </b>
	/// 
	/// </summary>
	struct ApiDevices
	{
		VulkanApi			vulkanApi			= nullptr;
		VulkanDevice*		vulkanDevice		= nullptr;
		VkDevice			vkDevice			= nullptr;
		VkPhysicalDevice	vkPhysicalDevice	= nullptr;
	 	VulkanWindow		window				= nullptr;
		VulkanSurface		surface				= nullptr;
		VulkanInstance		instance			= nullptr;
	};

	// device gets
	ApiDevices					getApiDevices()			{ return apiDevices;					}
	VulkanDevice*				getVulkanDevice()		{ return apiDevices.vulkanDevice;		}
	VkDevice					getVkDevice()			{ return apiDevices.vkDevice;			}
	VulkanInstance				getInstance()			{ return apiDevices.instance;			}
	VulkanSurface				getSurface()			{ return apiDevices.surface;			}
	VkPhysicalDevice			getPhysicalDevice()		{ return apiDevices.vkPhysicalDevice;	}


	void	init() override;
	void	quit() override;
	void*	getSwapChain() override { return swapChain; };

	static _VulkanApi* create(Window* window, const Device::Options& opts);

	
protected:
	_VulkanApi(Window* window, const Device::Options& opts);
	
		
	void createInstance();
	void createWindowSurface();

	std::vector<const char*> getRequiredExtensions(); 

	///  debugger
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);	
	bool checkValidationLayerSupport();
	void setupDebugMessenger();
	VkDebugUtilsMessengerEXT debugMessenger;

private:
	void		updateApiDeviceHelper();
	ApiDevices	apiDevices;
	
	/*
	Device				*device;
	WindowBase			*window;
	Device::Options		deviceOptions;
	const std::string	apiName;

	RenderingPipeline	*renderingPipeline;
	SwapChain			*swapChain;
	*/
};

using VulkanDevices		= _VulkanApi::ApiDevices;
