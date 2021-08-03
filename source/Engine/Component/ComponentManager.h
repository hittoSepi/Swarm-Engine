#pragma once
#ifndef COMPONENT_MANAGER
#define COMPONENT_MANAGER

#include "Core/typedefs.h"


/// <summary>
/// Component Manager class for <c>Component</c>\n
/// </summary>
class   ComponentManager: public Singleton<ComponentManager>
{
public:
	ComponentManager();
	~ComponentManager();

	void OnInit();
	void BeforeQuit();
	void OnUpdate();
	void OnRender();

	void Add(Component* obj);
	void Remove(IntHash id);

private:
	void AddObjects();
	void RemoveObjects();
	
	Components*		components;
	ComponentList*	addList;
	HashList*		removeList;
};

#endif
