#pragma once
#ifndef C_CGAME_OBJECT_C_
#define C_CGAME_OBJECT_C_

#include "Engine/BaseObject.h"


class GameObject: public BaseObject
{
public:
	friend class GameObjectManager;
	friend class Component;

	GameObject();
	GameObject(const std::string& name);
	
	virtual void OnInit()       override;
	virtual void OnUpdate()     override;
	virtual void OnRender()     override;
	virtual void BeforeQuit()   override;
	virtual void OnCreated()    override;
	virtual     ~GameObject()	override;

	// setters
	void SetRoot(GameObject* root)				{ root			= root; }
	void SetParent(GameObject* parent)			{ parent		= parent; }
	void SetChildrens(GameObjects* childrens)	{ childrens		= childrens; }
	void SetComponents(Components* components)	{ components	= components; }

	// getters
	GameObject*		GetRoot() { return parent; }
	GameObject*		GetParent() { return parent; }
	GameObjects*	GetChildrens() { return childrens; }
	Components*		GetComponents() { return components; }

	// adders
	void AddChildren(GameObject* go);					/// <summary>
	void AddComponent(Component* comp);					/// Add Component to <c>GameObject.</c>
	GameObject*	clone();								

	static 	GameObject*	create(std::string name);		/// Create new GameObject
	static GameObject* Create(std::string name);
protected:
	bool atPool						= false;			// GameObjectManager sets to true when added to pool.
private:
	GameObjects*	childrens			= nullptr;
	GameObject*		parent				= nullptr;
	GameObject*		root				= nullptr;
	Components*		components			= nullptr;

};



#endif
