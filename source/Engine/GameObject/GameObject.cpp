#include "pch.h"



GameObject::GameObject():
	BaseObject("error")
{
	should_not_get_here();
	//LogError(std::string("Don't create GameObjects with default constructor!"));
}

GameObject::GameObject(const std::string &name):
	BaseObject(name)
{
	OnInit();
}

GameObject::~GameObject()
{
	delete(childrens);
	delete(components);
}

void GameObject::BeforeQuit()
{
	SAFE_DELETE(childrens);
	SAFE_DELETE(components);
}


void GameObject::OnInit() {
	childrens = new GameObjects();
	components = new Components();
}

void GameObject::OnCreated()
{

}


void GameObject::OnUpdate()
{


}

void GameObject::OnRender()
{

}

void GameObject::AddChildren(GameObject* go) {
	childrens->try_emplace(go->GetID(), go);
}

void GameObject::AddComponent(Component* comp) {
	components->try_emplace(comp->GetID(), comp);
}

GameObject* GameObject::clone()						{ return new GameObject(*this); }
GameObject* GameObject::Create(std::string name)	{ return new GameObject(name);  } 

