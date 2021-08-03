#pragma once
#include "pch.h"


bool SAFE_DELETE(Components* objList)
{
	ComponentIt iter = objList->begin();
	while(iter !=  objList->end())
	{
		delete &iter;
	    iter++;
	}
	objList->clear();
	delete objList;
	return true;
}

bool SAFE_DELETE(HashList* objList)
{
	HashListIt iter = objList->begin();
	while(iter != objList->end())
	{
	     delete &iter;
	     iter++;
	}
	objList->clear();
	delete objList;
	return true;
}

bool SAFE_DELETE(GameObjectList* objList)
{
	GameObjectListIt iter = objList->begin();
	while(iter != objList->end())
	{
	     delete &iter;
	     iter++;
	}
	objList->clear();
	delete objList;
	return true;
}


bool SAFE_DELETE(GameObjects* objList)
{
	GameObjectPoolIt iter = objList->begin();
	while(iter != objList->end())
	{
	     delete &iter;
	     iter++;
	}
	objList->clear();
	delete objList;
	return true;
}


