#pragma once

class Component;
class GameObject;


typedef   size_t IntHash;

typedef   std::list<IntHash> HashList;

typedef   std::map<IntHash, Component*> Components;
typedef   std::list<Component*> ComponentList;

typedef   std::list<GameObject*> GameObjectList;
typedef   std::map<IntHash, GameObject*> GameObjectPool;
typedef   std::map<IntHash, GameObject*> GameObjects;

typedef   std::map<IntHash, Component*>::iterator ComponentIt;
typedef   std::map<IntHash, GameObject*>::iterator GameObjectPoolIt;

typedef   std::list<GameObject *>::iterator GameObjectListIt;
typedef   std::list<IntHash>::iterator HashListIt;
