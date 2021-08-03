#pragma once


#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#pragma comment(lib, "vulkan-1.lib")

#include "Core/Vulkan/VulkanDevice.h"
#include "Core/Vulkan/VulkanExtensions.h"
