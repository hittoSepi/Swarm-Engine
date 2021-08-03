#pragma once
#include "pch.h"
#ifndef C_ENGINE_CPP
#define C_ENGINE_CPP

#include "Utils/Debug/Debug.h"
#include "Engine/Engine.h"
#include "Engine/Component/ComponentManager.h"
#include "Engine/GameObject/gameObjectManager.h"

Engine::Engine()
{}


void Engine::OnInit()
{
	LogInfo("Starting up.")

	gameObjectManager	= &GameObjectManager::GetInstance();
	componentManager	= &ComponentManager::GetInstance();
}

void Engine::BeforeQuit()
{
	LogInfo("Finishing up.");
	gameObjectManager->BeforeQuit();
	componentManager->BeforeQuit();
}

void Engine::OnUpdate()
{
	gameObjectManager->OnUpdate();
	componentManager->OnUpdate();
}
void Engine::OnRender()
{
	gameObjectManager->OnRender();
	componentManager->OnRender();
}

Engine::~Engine()
{

}


IntHash Engine::GetId(std::string input) {
	return std::hash<std::string>{}(input);
}

GameObjectManager* Engine::GetGameObjectManager() { return gameObjectManager; }
ComponentManager*  Engine::GetComponentManager()  { return componentManager;}	


#endif
