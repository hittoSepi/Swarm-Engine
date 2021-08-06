#include "pch.h"
#include "Core/Vulkan/SwarmVulkan.h"
#include "Core/Vulkan/VulkanApi.h"
#include "Core/Vulkan/Debug/VulkanDebug.h"

using namespace std;

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};


_VulkanApi* _VulkanApi::create(Window* window, const Device::Options& opts)
{
	LogInfo("");
	if (window == nullptr) {
		LogWarning("Window is nullptr");
		return nullptr;
	}
	return new _VulkanApi(window, opts);
}


_VulkanApi::_VulkanApi(Window* window, const Device::Options& opts) :
	RenderingApi("Vulkan", window, opts)
{
}

void _VulkanApi::init()
{
	LogInfo("");
	apiDevices.vulkanApi = this;

	
	// Create Vulkan instance
	createInstance();

	// Debugging
	setupDebugMessenger();

	// Surface to attach window
	createWindowSurface();

	
	LogInfo("Create device.");
	
	setDevice(VulkanDevice::create(window, apiDevices.instance, apiDevices.surface, deviceOptions));
	device->init();

	updateApiDeviceHelper();
	
	LogInfo("RenderingPipeline.");
	renderingPipeline = new VulkanRenderingPipeline(VulkanRenderingPipeline::CreateOptions(this));
	renderingPipeline->init();
	
	LogDebug("end.");
}


void _VulkanApi::updateApiDeviceHelper()
{
	apiDevices.vulkanDevice			= dynamic_cast<VulkanDevice*>(device);
	apiDevices.vkDevice				= (VkDevice)(apiDevices.vulkanDevice->getApiDevice());
	apiDevices.vkPhysicalDevice		= apiDevices.vulkanDevice->getPhysicalDevice();
	apiDevices.window				= (VulkanWindow)window;
}


void _VulkanApi::quit()
{
	LogInfo("");
	LogVerbose("Deleting RenderingPipeline.");
	if (renderingPipeline != nullptr)
		renderingPipeline->quit();


	LogVerbose("Deleting WindowSurface.");
	vkDestroySurfaceKHR(apiDevices.instance, apiDevices.surface, nullptr);

	
	if (deviceOptions.enableValidationLayers) {
		LogVerbose("Deleting DebugMessenger.");
		DestroyDebugUtilsMessengerEXT(apiDevices.instance, debugMessenger, nullptr);
	}

	
	LogVerbose("Deleting VulkanDevice.");
	if (device != nullptr){
		auto device_ = dynamic_cast<VulkanDevice*>(device);
		device->quit();
		delete device;
	}

	
	LogVerbose("Deleting Instance.");
	vkDestroyInstance(apiDevices.instance, nullptr);


	LogDebug("end.");
}

void _VulkanApi::createInstance()
{
	LogInfo("");
	if (deviceOptions.enableValidationLayers && !checkValidationLayerSupport()) {
		LogError(std::string("validation layers requested, but not available!"));
	}

	
	VkApplicationInfo appInfo{};
	appInfo.sType				= VK_INFO::SW_VULKAN_APP_INFO;
	appInfo.pApplicationName	= window->getWindowTitle().c_str();
	appInfo.applicationVersion	= VK_MAKE_VERSION(1, 2, 0);
	appInfo.pEngineName			= SWARM_ENGINE_NAME;
	appInfo.engineVersion		= VK_MAKE_API_VERSION(0, SWARM_VERSION_MAJOR, SWARM_VERSION_MINOR, 0);
	appInfo.apiVersion			= VK_API;

	
	
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_CI::SW_VULKAN_INSTANCE;
	createInfo.pApplicationInfo = &appInfo;

	
	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();


	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (deviceOptions.enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else {
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	
	if (vkCreateInstance(&createInfo, nullptr, &apiDevices.instance) != VK_SUCCESS) {
		LogError(std::string("Failed to create instance."));
	}

	LogDebug("end.");
}


std::vector<const char*> _VulkanApi::getRequiredExtensions()
{
	LogInfo("");

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (deviceOptions.enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	return extensions;
}


bool _VulkanApi::checkValidationLayerSupport()
{
	LogInfo("");
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}
	return true;
}

void _VulkanApi::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	LogInfo("");
	createInfo = {};
	createInfo.sType = VK_CI::SW_VULKAN_DEBUG_MESSANGER;
	createInfo.messageSeverity = VULKAN_LOG_LEVEL;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = VulkanDebugCallback;
}


void _VulkanApi::setupDebugMessenger()
{
	LogInfo("");
	if (!deviceOptions.enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(apiDevices.instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		LogError(std::string("Failed to setup debug messenger!"));
	}
	LogDebug("end.");
}

void _VulkanApi::createWindowSurface()
{
	LogInfo("");
	if (glfwCreateWindowSurface(apiDevices.instance, (GLFWwindow*)window->getNativeWindow(), nullptr, &apiDevices.surface) != VK_SUCCESS) {
		LogError(std::string("Failed to create WindowSurface."));
	}
	LogDebug("end.");
}


