#pragma once
#define VK_API VK_API_VERSION_1_2  // Use Vulkan version 1.2

#ifdef _WIN32 
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#endif

#ifndef GENERATE_ENUM_STRINGS
#define DECL_ENUM_ELEMENT(ELEM, VAL) \
    		ELEM = VAL			

#define BEGIN_ENUM( ENUM_NAME ) typedef enum Vulkan##ENUM_NAME
#define END_ENUM( ENUM_NAME ) ENUM_NAME; \
            char* getString##ENUM_NAME(enum Vulkan##ENUM_NAME index);
#else
#define DECL_ENUM_ELEMENT( element ) #element
#define BEGIN_ENUM( ENUM_NAME ) char* gs_##ENUM_NAME [] =
#define END_ENUM( ENUM_NAME ) ; char* getString##ENUM_NAME(enum \
            tag##ENUM_NAME index){ return gs_##ENUM_NAME [index]; }
#endif

#define S(elem) SW_VULKAN_ ## elem
#define stringify( name ) # name

#define CREATE_ENUM(EnumName, ...)                   \
struct EnumName {                     \
    enum _enumerated { __VA_ARGS__ }; \
                                      \
    _enumerated  value;                \
                                      \
};

/// Vulkan Info Types
CREATE_ENUM(_VK_INFO,
	S(APP_INFO							= VK_STRUCTURE_TYPE_APPLICATION_INFO),
	S(COMMAND_BUFFER_ALLOC_INFO			= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO),
)

/// Vulkan Create Info Types
CREATE_ENUM(_VK_CI,
	S(DEBUG_MESSANGER					= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT),
	S(INSTANCE							= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO),
	S(DEVICE							= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO),
	S(EVENT								= VK_STRUCTURE_TYPE_EVENT_CREATE_INFO),
	S(DEVICE_QUEUE						= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO),
	S(BUFFER							= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO),
	S(RENDER_PASSS						= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO),
	S(IMAGE_VIEW						= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO),
	S(COMMAND_POOL						= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO),
	S(SWAPCHAIN							= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR),
	S(PIPELINE_LAYOUT					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO),
	S(PIPELINE_INPUT_ASSEMBLY			= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO),
	S(PIPELINE_DYNAMIC					= VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO),
	S(PIPELINE_VERTEX_INPUT				= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO),
	S(PIPELINE_VIEWPORT					= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO),
	S(PIPELINE_RASTERIZATION			= VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO),
	S(PIPELINE_MULTISAMPLE				= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO),
	S(PIPELINE_COLOR_BLEND				= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO),
	S(PIPELINE_SHADER_STAGE				= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO),
	S(SAMPLER							= VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO)
)

CREATE_ENUM(_VK_SHADER,
	VERTEX		= VK_SHADER_STAGE_VERTEX_BIT,
	GEOMETRY	= VK_SHADER_STAGE_GEOMETRY_BIT,
	FRAGMENT	= VK_SHADER_STAGE_FRAGMENT_BIT,
	COMPUTE		= VK_SHADER_STAGE_COMPUTE_BIT,
	ALL			= VK_SHADER_STAGE_ALL
)

typedef VkStructureType vkst;
#define VK_INFO (vkst)_VK_INFO
#define VK_CI (vkst)_VK_CI
#define VK_SHADER (VkShaderStageFlagBits)_VK_SHADER

#include "Core/Vulkan/VulkanApi.h"
#include "Core/Vulkan/VulkanDevice.h"

#include "Core/Vulkan/Shader/VulkanShaderModule.h"
#include "Core/Vulkan/Shader/VulkanShader.h"

#include "Core/Vulkan/Render/VulkanViewport.h"
#include "Core/Vulkan/Render/VulkanSwapChain.h"
#include "Core/Vulkan/Render/VulkanRenderPass.h"
#include "Core/Vulkan/Render/VulkanRenderPipeline.h"

#pragma comment(lib, "vulkan-1.lib")