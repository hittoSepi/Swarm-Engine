#pragma once
#include "pch.h"
#ifndef C_COMPONENT_CPP_
#define C_COMPONENT_CPP_

#include "Engine/Component/Component.h"
#include "Utils/debug/Debug.h"
#include "Utils/Helpers.h"


Component::Component()
{
	SetName("Component_");
} 


Component::Component(const std::string& name)
{
	SetName(name);
}

Component::~Component()
{
	//delete (AttachedGameObject);
}

void Component::BeforeQuit()
{
	//AttachedGameObject->BeforeQuit();
}

void Component::OnInit()
{
	if (AttachedGameObject == nullptr)
	{
		LogWarning("Component isn't attached to GameObject.");
	}
}

void Component::OnCreated()
{
}


void Component::OnUpdate()
{
}

void Component::OnRender()
{
}


int Component::SetIndex(const int idx)
{
	index = idx;
	return index;
}

/// Attach Component to Game”bject
void Component::SetGameObject(GameObject* gameobj) { AttachedGameObject = gameobj; }
GameObject* Component::GetAttachedGameObject() { return AttachedGameObject; }

#endif
