#pragma once
#ifndef C_CGAME_OBJECT_C_
#define C_CGAME_OBJECT_C_

#include "Engine/BaseObject.h"

class GameObject : public BaseObject
{
public:
	friend class SerializeAccess;
	friend class GameObjectManager;
	friend class Component;

	GameObject();
	GameObject(const std::string& name);

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnRender() override;
	virtual void BeforeQuit() override;
	virtual void OnCreated() override;
	virtual ~GameObject() override;

	// setters
	void setRoot(GameObject* _root) { root = _root; }
	void setParent(GameObject* _parent) { parent = _parent; }
	void setChildrens(GameObjects* _childrens) { childrens = _childrens; }
	void SetComponents(Components* _components) { components = _components; }

	// getters
	GameObject* GetRoot() { return parent; }
	GameObject* GetParent() { return parent; }
	GameObjects* GetChildrens() { return childrens; }
	Components* GetComponents() { return components; }

	// adders
	void AddChildren(GameObject* go);
	void AddComponent(Component* comp); /// Add Component to GameObject
	
	GameObject* clone();

	static GameObject* Create(std::string name); /// Create new GameObject


protected:
	bool atPool = false; // GameObjectManager sets to true when added to pool

	GameObjects* childrens = nullptr;
	GameObject* parent = nullptr;
	GameObject* root = nullptr;
	Components* components = nullptr;
};


#endif
