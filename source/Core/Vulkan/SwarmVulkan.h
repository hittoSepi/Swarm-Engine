#pragma once
#define VK_API VK_API_VERSION_1_2  // Use Vulkan version 1.2

#ifdef _WIN32 
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#endif

#include "Core/Vulkan/VulkanApi.h"
#include "Core/Vulkan/VulkanDevice.h"
#include "Core/Vulkan/VulkanShaderModule.h"
#include "Core/Vulkan/VulkanShader.h"
#include "Core/Vulkan/VulkanSwapChain.h"
#include "Core/Vulkan/VulkanRenderPipeline.h"

#pragma comment(lib, "vulkan-1.lib")