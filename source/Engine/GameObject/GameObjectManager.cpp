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
	gameObjects = new GameObjectPool();
	addList		= new GameObjectList();
	removeList	= new HashList();
	updateThread = new thread(GameObjectManager::UpdateThread);
	Running = true;
	updateThread->detach();
}

void GameObjectManager::UpdateThread()
{
	LogInfo("Started UpdateThread().");
	
	while (Running)
	{
		if (!addList->empty() || !removeList->empty() )
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
	SAFE_DELETE(gameObjects);
	SAFE_DELETE(addList);
	SAFE_DELETE(removeList);

	delete (updateThread);
}



void GameObjectManager::AddObjects()
{
	for (GameObjectListIt iter = addList->begin(); iter != addList->end(); ++iter)
	{
		GameObject *obj = (GameObject*)iter._Ptr;

		// Skip if some how already at pool
		if (obj->atPool == false)
		{
			obj->atPool = true;
			gameObjects->try_emplace(obj->getID(), obj);
			gameObjectsCreated++;
		}
	}
	addList->clear();
	
}



void GameObjectManager::RemoveObjects()
{
	HashListIt iter = removeList->begin();
	while(iter !=  removeList->end())
	{
		GameObjectPoolIt goDelIter = gameObjects->find(((GameObject*) &iter)->getID());
		gameObjects->erase(goDelIter);
	    delete &iter;
	    iter++;
	}
}


void GameObjectManager::Remove(GameObject *obj)
{
	if (lock.try_lock())
	{
		removeList->emplace_back(obj->getID());
		lock.unlock();
	}
}


void GameObjectManager::Add(GameObject *obj)
{
	if (lock.try_lock())
	{
		addList->emplace_back(obj);
		lock.unlock();
	}
}

GameObject *GameObjectManager::Create(std::string name)
{
	GameObject *newObject = new GameObject(name);
	if (lock.try_lock())
	{
		addList->emplace_back(newObject);
		lock.unlock();
	}
	return newObject;
}

#endif