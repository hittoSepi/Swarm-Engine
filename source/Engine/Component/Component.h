#pragma once
#ifndef C_COMPONENT_H
#define C_COMPONENT_H


class   Component
{
public:
	friend class ComponentManager;
	friend class GameObject;
	
	Component();
	Component(const std::string& name);
	virtual     ~Component();

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnCreated();
	virtual void BeforeQuit();


	// setters
	void    SetGameObject(GameObject* gameobj);
	int     SetIndex(const int idx);
	void	SetName(std::string name) { Name = name; }
	
	// getters
	int                 GetIndex()	{ return index; }
	IntHash				GetID()		{ return id; }
	std::string			GetName()	{ return Name; }
	GameObject			*GetAttachedGameObject();

	static Component* create(std::string name) { return new Component(name); }

private:
	IntHash			id		= -1;
	std::string		Name	= "";	
	int             index	= -1;
	GameObject		*AttachedGameObject = nullptr;
	

protected:
	void SetID(const IntHash _id) { id = _id; }

private:


};

#endif
