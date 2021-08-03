#include "pch.h"


BaseObject::BaseObject()
{
	LogError("Don't use with default constructor!");
}


BaseObject::BaseObject(std::string name)
{
	id = Engine::GetId(name);	
}


