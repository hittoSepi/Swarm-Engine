#include "pch.h"

using namespace std;

ComponentManager::ComponentManager()
{}


void ComponentManager::OnInit()
{
	LogInfo("Starting up.")
	components	= new Components();
	addList		= new ComponentList();
	removeList	= new HashList();
}

ComponentManager::~ComponentManager()
{
	SAFE_DELETE(addList);
	SAFE_DELETE(removeList);
	SAFE_DELETE(components);
}

void ComponentManager::BeforeQuit()
{
	LogInfo("Finishig up.")
}

void ComponentManager::OnUpdate()
{
	RemoveObjects();
	AddObjects();
}

void ComponentManager::OnRender()
{
}

void ComponentManager::AddObjects()
{
	if(!addList->empty()) {
		for_each(addList->begin(), addList->end(), [&](Component *object)
		{
			components->try_emplace(object->GetID(), object);
		});
		LogInfo("Items added: " + addList->size())
		addList->clear();
	}
}

void ComponentManager::RemoveObjects()
{
	if (removeList->empty()) {
		for_each(removeList->begin(), removeList->end(), [&](IntHash hash)
		{
			auto _it = components->find(hash);
			components->erase(_it);
		});
		LogInfo("Items added: " + removeList->size())
		removeList->clear();
	}
}

void ComponentManager::Remove(IntHash id)
{
	removeList->emplace_back(id);
}

void ComponentManager::Add(Component *obj)
{
	addList->emplace_back(obj);
}

