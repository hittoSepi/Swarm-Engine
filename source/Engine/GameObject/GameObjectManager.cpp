#pragma once
#include "pch.h"

#ifndef C_GAMEOBJECT_MANAGER_CPP_
#define C_GAMEOBJECT_MANAGER_CPP_


using namespace std;

GameObjectManager::GameObjectManager()
{
	
}


void GameObjectManager::OnInit()
{
	GameObjects = new GameObjectPool();
	AddList		= new GameObjectList();
	RemoveList	= new HashList();
	updateThread = new thread(GameObjectManager::UpdateThread);
	Running = true;
	updateThread->detach();
}

void GameObjectManager::UpdateThread()
{
	LogInfo("Started UpdateThread().");
	
	while (Running)
	{
		if (!AddList->empty() || !RemoveList->empty() )
		{
			RemoveObjects();
			AddObjects();
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void GameObjectManager::OnUpdate()
{
}

void GameObjectManager::OnRender()
{
}

/// <summary>
/// Wait for UpdateThread to join back.
/// </summary>
void GameObjectManager::BeforeQuit()
{

	LogInfo("GameObjectManager: Quitting thread");
	Running = false;
}

GameObjectManager::~GameObjectManager()
{
	SAFE_DELETE(GameObjects);
	SAFE_DELETE(AddList);
	SAFE_DELETE(RemoveList);

	delete (updateThread);
}



void GameObjectManager::AddObjects()
{
	GameObjectListIt iter;
	for (iter = AddList->begin(); iter != AddList->end(); ++iter)
	{
		GameObject *obj = (GameObject*)iter._Ptr;

		// Skip if some how already at pool
		if (obj->atPool == false)
		{
			obj->atPool = true;
			GameObjects->try_emplace(obj->GetID(), obj);
			gameObjectsCreated++;
		}
	}
	AddList->clear();
	
}



void GameObjectManager::RemoveObjects()
{
	HashListIt iter = RemoveList->begin();
	while(iter !=  RemoveList->end())
	{
		GameObjectPoolIt goDelIter = GameObjects->find(((GameObject*) &iter)->GetID());
		GameObjects->erase(goDelIter);
	    delete &iter;
	    iter++;
	}
}


void GameObjectManager::Remove(GameObject *obj)
{
	if (lock.try_lock())
	{
		RemoveList->emplace_back(obj);
		lock.unlock();
	}
}


void GameObjectManager::Add(GameObject *obj)
{
	if (lock.try_lock())
	{
		AddList->emplace_back(obj);
		lock.unlock();
	}
}

GameObject *GameObjectManager::Create(std::string name)
{
	GameObject *newObject = new GameObject(name);
	if (lock.try_lock())
	{
		AddList->emplace_back(newObject);
		lock.unlock();
	}
	return newObject;
}

#endif