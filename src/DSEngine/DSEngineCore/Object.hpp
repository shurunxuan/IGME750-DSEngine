#pragma once
#pragma warning(disable:4251)

#include <list>
#include <string>
#include <typeinfo>

#define BOOST_ALLOW_DEPRECATED_HEADERS
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "Component.hpp"
#include "Transform.hpp"
class Scene;

class Object
{
public:
	Object(Scene* scene);
	Object(Scene* scene, std::string name);
	virtual ~Object();

	boost::uuids::uuid GetInstanceID() const;

	template <class T>
	T* GetComponent();

	template <class T>
	std::list<T*> GetComponents();

	template <class T>
	T* AddComponent();

	template <class T>
	void RemoveComponent(T* component);

	virtual void Update(float deltaTime, float totalTime);

	Scene* GetScene();

	friend bool operator==(const Object& v1, const Object& v2);
	friend bool operator!=(const Object& v1, const Object& v2);

private:
	boost::uuids::uuid id;
	Scene* owner;

	std::list<Component*> components;
public:
	std::string name;
	bool isHidden;

	Transform* transform;
};

template <class T>
T* Object::GetComponent()
{
	for (Component* component : components)
	{
		T* cast = dynamic_cast<T*>(component);
		if (cast != nullptr) return cast;
		//if (typeid(component) == typeid(T*))
		//	return static_cast<T*>(component);
	}
	return nullptr;
}

template <class T>
std::list<T*> Object::GetComponents()
{
	std::list<T*> result;
	for (Component* component : components)
	{
		T* cast = dynamic_cast<T*>(component);
		if (cast != nullptr) result.push_back(cast);
		//if (typeid(component) == typeid(T*))
		//	result.push_back(static_cast<T*>(component));
	}
	return result;
}

template <class T>
T* Object::AddComponent()
{
	T* newComponent = new T(this);

	components.push_back(newComponent);
	newComponent->Start();

	return newComponent;
}

template <class T>
void Object::RemoveComponent(T * component)
{
	if (component == transform) return;
	components.remove(component);
	delete component;
}

inline void Object::Update(float deltaTime, float totalTime)
{
	for (Component* component : components)
	{
		if (component->isActive)
			component->Update(deltaTime, totalTime);
	}
}

inline Scene* Object::GetScene()
{
	return owner;
}

inline Object::Object(Scene * scene)
	: id(boost::uuids::random_generator()())
	, name("Object")
	, owner(scene)
	, isHidden(false)
{
	transform = AddComponent<Transform>();
}

inline Object::Object(Scene * scene, std::string name)
	: id(boost::uuids::random_generator()())
	, name(std::move(name))
	, owner(scene)
	, isHidden(false)
{
	transform = AddComponent<Transform>();
}

inline Object::~Object()
{
	for (Component* component : components)
	{
		delete component;
	}
	components.clear();
}

inline boost::uuids::uuid Object::GetInstanceID() const
{
	return id;
}

inline bool operator==(const Object & v1, const Object & v2)
{
	return v1.id == v2.id;
}

inline bool operator!=(const Object & v1, const Object & v2)
{
	return v1.id != v2.id;
}
