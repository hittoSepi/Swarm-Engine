#pragma once
#ifndef SWARM_H
#define SWARM_H
#define SWARM_ENGINE_NAME "Swarm Engine"
#define SWARM_VERSION_MAJOR 0
#define SWARM_VERSION_MINOR 2


#define SWARM_USE_GLM true
#define VK_API VK_API_VERSION_1_2  // Use Vulkan version 1.2

#include "SwarmStdHeaders.h"


inline void should_not_get_here()
{
	assert("should_not_get_here");
}




#if SWARM_USE_GLM
#define GLM_FORCE_RADIANS
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
using float2 = glm::vec2;
using float3 = glm::vec3;
using mat4 = glm::mat4;
#endif

#ifdef _DEBUG
#define SWARM_LOG_LEVEL_DEBUG 0
#define SWARM_LOG_LEVEL_VERBOSE 1
#define SWARM_LOG_LEVEL_INFO 2
#define SWARM_LOG_LEVEL_WARNING 3
#define SWARM_LOG_LEVEL_ERROR 4

#define SWARM_LOG_LEVEL SWARM_LOG_LEVEL_VERBOSE
#define SWARM_LOG_FILE_LEVEL SWARM_LOG_LEVEL_VERBOSE

#include "Utils/Debug/Debug.h"
#endif


////////////////////////////////////////////////////
/// CORE INCLUDES
////////////////////////////////////////////////////
#include "Core/Framework.h"
#include "Core/Window/WindowBase.h"
#include "Core/Window/Window.h"
#include "Core/Device.h"
#include "Core/Renderer.h"
#include "Core/Input/InputEvents.h"
#include "Core/ThreadPool/ThreadPool.h"
#include "Core/Singleton/Singleton.h"

// Rendering abstractions
#include "Core/Rendering/Viewport.h"
#include "Core/Rendering/SwapChain.h"
#include "Core/Rendering/RenderPass.h"
#include "Core/Rendering/RenderingPipeline.h"
#include "Core/Rendering/RenderingApi.h"

// Shader abstractions
#include "Core/Shader/ShaderCache.h"
#include "Core/Shader/Shader.h"
#include "Core/Shader/ShaderHelpers.h"

// Graphics api(s)
#include "Core/Vulkan/SwarmVulkan.h"

// Application framework
#include "Core/Application.h"

////////////////////////////////////////////////////
/// UTILITY INCLUDES
////////////////////////////////////////////////////


#include "Utils/Helpers.h"
#include "Utils/Locale/Utf8.h"
#include "Utils/time/Clock.h"
#include "Utils/Time/FrameRate.h"

#include "Utils/Math/Rect.h"

#ifdef _WIN32
#include "Utils/Process/WinProcess.h"
#endif



////////////////////////////////////////////////////
/// ENGINE INCLUDES
////////////////////////////////////////////////////
#include "Engine/Engine.h"
/// GameObjects
#include "Engine/BaseObject.h"
#include "Engine/GameObject/GameObject.h"
#include "Engine/GameObject/gameObjectManager.h"
/// components
#include "Engine/Component/Component.h"
#include "Engine/Component/ComponentManager.h"
/// Scene


////////////////////////////////////////////////////
/// ADD SOME LIBRARIES
////////////////////////////////////////////////////
#ifdef _WIN32
#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "glew32.lib")
#endif

// typedefs
#include "Core/typedefs.h"


#endif
