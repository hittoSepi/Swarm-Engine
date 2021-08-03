#include "pch.h"
#include "Core/Vulkan/VulkanExtensions.h"

#include <GLFW/glfw3.h>

std::vector<const char*> VulkanExtensions::getVulkanExtensions(std::vector<const char*> requiredExtensions)
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	for(auto ext: requiredExtensions)
	{
		extensions.push_back(ext);
	}


  

	return extensions;
}