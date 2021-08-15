#pragma once
#pragma warning(disable: 4267) // ignore size_t to int32_t warning

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


#define CREATE_ENUM(EnumName, ...)                  \
struct EnumName {									\
    enum _enumerated { __VA_ARGS__ };				\
													\
    _enumerated  value;								\
													\
};

/// Vulkan Info Types
CREATE_ENUM(_VK_INFO,
	S(APP_INFO							= VK_STRUCTURE_TYPE_APPLICATION_INFO),
	S(COMMAND_BUFFER_ALLOC_INFO			= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO),
)


CREATE_ENUM(VulkanFormat,
	RGBA32								= VK_FORMAT_R32G32B32A32_UINT, // 32 bytes per channel
	RGB32								= VK_FORMAT_R32G32B32_UINT,
	RG32								= VK_FORMAT_R32G32_UINT,
	R32									= VK_FORMAT_R32_UINT,
	RGBA16								= VK_FORMAT_R16G16B16A16_UINT, // 16 bytes per channel
	RGB16								= VK_FORMAT_R16G16B16_UINT,
	RG16								= VK_FORMAT_R16G16_UINT,
	R16									= VK_FORMAT_R16_UINT,
	RGBA8								= VK_FORMAT_B8G8R8A8_SRGB,		// 8 bytes per channel
	RGB8								= VK_FORMAT_R8G8B8_SRGB,
	RG8									= VK_FORMAT_R8G8_SRGB,
	R8									= VK_FORMAT_R8_SRGB
)


CREATE_ENUM(_VK_CI, 
	S(DEBUG_MESSANGER					= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT),
	S(INSTANCE							= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO),
	S(DEVICE							= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO),
	S(EVENT								= VK_STRUCTURE_TYPE_EVENT_CREATE_INFO),
	S(DEVICE_QUEUE						= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO),
	S(BUFFER							= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO),
	S(RENDER_PASS						= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO),
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
	S(SAMPLER							= VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO),
	S(SHADER_MODULE						= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO),
	S(PIPELINE_SHADER_STAGE				= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO)
)


CREATE_ENUM(_BIND, 
	S(GRAPHICS		= VK_PIPELINE_BIND_POINT_GRAPHICS),
	S(COMPUTE		= VK_PIPELINE_BIND_POINT_COMPUTE),
	S(RAYTRACING	= VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR),
	S(RAYTRACING_NV	= VK_PIPELINE_BIND_POINT_RAY_TRACING_NV),
);


CREATE_ENUM(_VK_SHADER,
	VERTEX								= VK_SHADER_STAGE_VERTEX_BIT,
	GEOMETRY							= VK_SHADER_STAGE_GEOMETRY_BIT,
	FRAGMENT							= VK_SHADER_STAGE_FRAGMENT_BIT,
	COMPUTE								= VK_SHADER_STAGE_COMPUTE_BIT,
	ALL									= VK_SHADER_STAGE_ALL
);


CREATE_ENUM(VulkanImageType,
	TEX1D								= VK_IMAGE_VIEW_TYPE_1D,
    TEX2D								= VK_IMAGE_VIEW_TYPE_2D,
    TEX3D								= VK_IMAGE_VIEW_TYPE_3D,
	CUBE								= VK_IMAGE_VIEW_TYPE_CUBE,
    ARRAY1D								= VK_IMAGE_VIEW_TYPE_1D_ARRAY,
    ARRAY2D								= VK_IMAGE_VIEW_TYPE_2D_ARRAY ,
    ARRAY3D								= VK_IMAGE_VIEW_TYPE_CUBE_ARRAY,
	MAX_ENUM							= VK_IMAGE_VIEW_TYPE_MAX_ENUM ,
)
class Window;
typedef Window* VulkanWindow;
typedef VkStructureType vkst;

#define VK_SWIZZLE VK_COMPONENT_SWIZZLE_IDENTITY
#define VK_INFO (vkst)_VK_INFO
#define VK_SHADER (VkShaderStageFlagBits)_VK_SHADER
#define VK_FORMAT (VkFormat)VulkanFormat
#define VK_IMG (VkImageViewType)VulkanImageType
#define vk_ci (vkst)_VK_CI
#define VK_CI (vkst)_VK_CI
#define BIND (VkPipelineBindPoint)_BIND
#define vk_img_type(img) (VkImageViewType)img
#define vk_img_format(format) (VkFormat)format




#include "Core/Vulkan/VulkanApi.h"
#include "Core/Vulkan/VulkanDevice.h"


#include "Core/Vulkan/VulkanCommandPool.h"

// shaders
#include "Core/Vulkan/Shader/VulkanShaderModule.h"
#include "Core/Vulkan/Shader/VulkanShader.h"

// textures
#include "Core/Vulkan/Texture/VulkanTexture.h"

// vulkan rendering includes
#include "Core/Vulkan/Render/VulkanViewport.h"
#include "Core/Vulkan/Render/VulkanImageView.h"
#include "Core/Vulkan/Render/VulkanFrameBuffer.h"
#include "Core/Vulkan/Render/VulkanSwapChain.h"
#include "Core/Vulkan/Render/VulkanRasterizer.h"
#include "Core/Vulkan/Render/VulkanRenderPipeline.h"

// vulkan renderpass parts
#include "Core/Vulkan/Render/RenderPass/VulkanAttachReference.h"
#include "Core/Vulkan/Render/RenderPass/VulkanAttachment.h"
#include "Core/Vulkan/Render/RenderPass/VulkanSubPassDependency.h"
#include "Core/Vulkan/Render/RenderPass/VulkanSubPass.h"
#include "Core/Vulkan/Render/RenderPass/VulkanRenderPass.h"

#include "Core/Vulkan/Vertex/Vertex.h"
#include "Core/Vulkan/Vertex/VulkanVertexBuffer.h"

// render passes
#include "Core/Vulkan/RenderPasses/VulkanSimpleRenderPass.h"
#include "Core/Vulkan/RenderPasses/VulkanPBSRenderPass.h"

#pragma comment(lib, "vulkan-1.lib")