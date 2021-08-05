#include "pch.h"
#include "Core/Vulkan/VulkanApi.h"

#include "VulkanDebug.h"

using namespace std;

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};


VulkanApi* VulkanApi::create(Window* window, const Device::Options& opts)
{
	LogInfo("");
	if (window == nullptr) {
		LogWarning("Window is nullptr");
		return nullptr;
	}
	return new VulkanApi(window, opts);
}


VulkanApi::VulkanApi(Window* window, const Device::Options& opts) :
	RenderingApi("Vulkan", window, opts)
{
}

void VulkanApi::init()
{
	LogInfo("")
	createInstance();

	setupDebugMessenger();

	createWindowSurface();
	createSwapChain();

	LogInfo("Create device.")
		setDevice(VulkanDevice::create(window, instance, surface, deviceOptions));
	device->init();

	LogInfo("RenderingPipeline.");
	renderingPipeline = new VulkanRenderingPipeline((VulkanDevice*)device);
	renderingPipeline->init();

	LogDebug("end.");
}

void VulkanApi::quit()
{
	LogInfo("");

	if (renderingPipeline != nullptr)
		renderingPipeline->quit();

	if (device != nullptr)
		device->quit();

	LogVerbose("Deleting WindowSurface.");

	vkDestroySurfaceKHR(instance, surface, nullptr);

	if (deviceOptions.enableValidationLayers) {
		LogVerbose("Deleting DebugMessenger.");
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	LogVerbose("Deleting Instance.");
	vkDestroyInstance(instance, nullptr);

	LogDebug("end.");
}

void VulkanApi::createInstance()
{
	LogInfo("");
	if (deviceOptions.enableValidationLayers && !checkValidationLayerSupport()) {
		LogError(std::string("validation layers requested, but not available!"));
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = window->getWindowTitle().c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = SWARM_ENGINE_NAME;
	appInfo.engineVersion = VK_MAKE_VERSION(SWARM_VERSION_MAJOR, SWARM_VERSION_MINOR, 0);
	appInfo.apiVersion = VK_API;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
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

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		LogError(std::string("Failed to create instance."));
	}

	LogDebug("end.");

}

std::vector<const char*> VulkanApi::getRequiredExtensions()
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


bool VulkanApi::checkValidationLayerSupport()
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

void VulkanApi::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	LogInfo("");
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VULKAN_LOG_LEVEL;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = VulkanDebugCallback;
}


void VulkanApi::setupDebugMessenger()
{
	LogInfo("");
	if (!deviceOptions.enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		LogError(std::string("Failed to setup debug messenger!"));
	}
	LogDebug("end.");
}

void VulkanApi::createWindowSurface()
{
	LogInfo("");
	if (glfwCreateWindowSurface(instance, (GLFWwindow*)window->getNativeWindow(), nullptr, &surface) != VK_SUCCESS) {
		LogError(std::string("Failed to create WindowSurface."));
	}
	LogDebug("end.");
}


QueueFamilyIndices VulkanApi::findQueueFamilies()
{
	LogInfo("");
	
	const VkPhysicalDevice& device = getPhysicalDevice();

	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}


void VulkanApi::createSwapChain()
{
	LogInfo("");
	swapChain = new VulkanSwapChain(this);
}
