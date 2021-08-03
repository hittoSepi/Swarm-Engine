#pragma once
#ifndef BASE_OBJECT_H_
#define BASE_OBJECT_H_

#include "Core/typedefs.h"

/// <summary>
/// Base Entity object
/// </summary>
class   BaseObject
{
public:
	BaseObject(std::string name);

	virtual ~BaseObject(){};
	virtual void OnInit() = 0;
	virtual void OnCreated() = 0; // Triggers once when spawned to scene
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void BeforeQuit() = 0;

	void SetName(std::string name) { Name = name; }

	IntHash GetID() { return id; }
	std::string GetName() { return Name; }

protected:
	void SetID(IntHash id) { this->id = id; }

private:
	BaseObject();
	IntHash id = -1;
	std::string Name = "";
};
#endif
