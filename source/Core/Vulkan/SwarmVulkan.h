#pragma once

#define VK_API VK_API_VERSION_1_2
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#pragma comment(lib, "vulkan-1.lib")

#include "Core/Vulkan/VulkanDevice.h"
#include "Core/Vulkan/VulkanShader.h"
#include "Core/Vulkan/VulkanGraphicsPipeline.h"