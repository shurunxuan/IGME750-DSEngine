#pragma once
#pragma warning(disable:4251)

#include <list>
#include "Object.hpp"

class Object;

class Scene
{
public:
	Scene();
	~Scene();

	Object* AddObject(std::string name = "GameObject");
	Object* Instantiate(Object* obj);
	void DestroyObject(Object* obj);

	std::list<Object*> allObjects;
};

inline Scene::Scene()
{
}

inline Scene::~Scene()
{
}

inline Object* Scene::AddObject(std::string name)
{
	Object* newObj = new Object(this, name);
	allObjects.push_back(newObj);
	return newObj;
}


inline Object* Scene::Instantiate(Object* obj)
{
}

inline void Scene::DestroyObject(Object* obj)
{
	const auto result = std::find(allObjects.begin(), allObjects.end(), obj);
	if (result == allObjects.end()) return;
	allObjects.erase(result);
}
