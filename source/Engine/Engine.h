#pragma once
#ifndef C_ENGINE_H_
#define C_ENGINE_H_
#include "Core/Singleton/Singleton.h"
#include "Engine/Component/ComponentManager.h"
#include "Engine/GameObject/gameObjectManager.h"

class   Engine : public Singleton<Engine>
{
public:
	friend class Singleton<Engine>;

	Engine();
	~Engine();

	void OnInit();
	void OnUpdate();
	void OnRender();
	void BeforeQuit();

	GameObjectManager* GetGameObjectManager();
	ComponentManager*  GetComponentManager();

	static IntHash GetId(std::string input);

private:
	GameObjectManager*	gameObjectManager;
	ComponentManager*	componentManager;
};
#endif
