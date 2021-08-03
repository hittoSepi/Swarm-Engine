#pragma once
#ifndef SWARM_H
#define SWARM_H

#define SWARM_VULKAN 1

#include "SwarmPrerequires.h"
#include "SwarmStdHeaders.h"

////////////////////////////////////////////////////
/// CORE INCLUDES
////////////////////////////////////////////////////
#include "Core/Framework.h"
//#include "Core/SwarmApp.h"
#include "Core/WindowBase.h"
#include "Core/Window.h"
#include "Core/Renderer.h"
#include "Core/Input/InputEvents.h"
#include "Core/ThreadPool/ThreadPool.h"
#include "Core/Singleton/Singleton.h"
#include "Core/Locale/Utf8.h"


////////////////////////////////////////////////////
/// UTILITY INCLUDES
////////////////////////////////////////////////////
#include "Utils/Helpers.h"
#ifdef _DEBUG
#include "Utils/Debug/Debug.h"
#endif
#include "Utils/time/Clock.h"
#include "Utils/Time/FrameRate.h"


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
