#pragma once
#pragma warning(disable:4251)

#include <list>
#include "Object.hpp"

class Scene
{
public:
	Scene();
	~Scene();

	Object* AddObject(std::string name = "GameObject");
	Object* Instantiate(Object* obj);
	void DestroyObject(Object* obj);

	void Update(float deltaTime, float totalTime);
private:
	std::list<Object*> allObjects;
};


inline Scene::Scene()
{
}

inline Scene::~Scene()
{
	for (Object* object : allObjects)
	{
		delete object;
	}
	allObjects.clear();
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
	delete obj;
}

inline void Scene::Update(float deltaTime, float totalTime)
{
	for (Object* object : allObjects)
	{
		object->Update(deltaTime, totalTime);
	}
}

