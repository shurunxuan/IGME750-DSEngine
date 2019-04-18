/**
 * @file Object.hpp
 * @author Victor Shu
 * @brief This file contains the class that represents
 * the game object
 * 
 * @version 0.1
 * @date 2019/03/09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

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

/**
 * @brief A game object
 * 
 */
class Object
{
public:
	/**
	 * @brief Construct a new Object with the name "Object"
	 * 
	 * @param scene The scene that owns the object
	 */
	Object(Scene* scene);
	/**
	 * @brief Construct a new Object with a name
	 * 
	 * @param scene The scene that owns the object
	 * @param name The name of the object
	 */
	Object(Scene* scene, std::string name);
	/**
	 * @brief Destroy the Object
	 * 
	 */
	virtual ~Object();

	/**
	 * @brief Get the Instance ID of the object
	 * 
	 * @return boost::uuids::uuid The UUID of the object
	 */
	boost::uuids::uuid GetInstanceID() const;

	/**
	 * @brief Get the first Component of a specific type
	 * 
	 * @tparam T The type of the component
	 * @return T* The pointer points to the component, nullptr if nothing is found
	 * 
	 * @todo We should consider if we want to use RTTI and dynamic_cast for this.
	 */
	template <class T>
	T* GetComponent();

	/**
	 * @brief Get all Components of a specific type
	 * 
	 * @tparam T The type of the component
	 * @return std::list<T*> A list of pointers point to the components
	 */
	template <class T>
	std::list<T*> GetComponents();

	/**
	 * @brief Add a component
	 * 
	 * @tparam T The type of the component
	 * @return T* The pointer of the added component
	 */
	template <class T>
	T* AddComponent();

	/**
	 * @brief Remove a component
	 * 
	 * Nothing happens if there's no such component
	 * 
	 * @tparam T The type of the component
	 * @param component The pointer of the component to be removed
	 */
	template <class T>
	void RemoveComponent(T* component);

	/**
	 * @brief Called when the scene updates
	 * 
	 * Update all the components
	 * 
	 * @param deltaTime The time that a frame costs
	 * @param totalTime The total time from the beginning of the application
	 */
	virtual void Update(float deltaTime, float totalTime);

	/**
	 * @brief Get the scene that owns this object
	 * 
	 * @return Scene* The pointer of the scene
	 */
	Scene* GetScene();

	/**
	 * @brief Tell if the UUIDs of two objects are identical
	 * 
	 * @param v1 An object
	 * @param v2 Another object
	 * @return bool If the UUIDs are identical
	 */
	friend bool operator==(const Object& v1, const Object& v2);
	/**
	 * @brief Tell if the UUIDs of two objects are not identical
	 * 
	 * @param v1 An object
	 * @param v2 Another object
	 * @return bool If the UUIDs are not identical
	 */
	friend bool operator!=(const Object& v1, const Object& v2);

private:
	/**
	 * @brief The UUID of the object
	 * 
	 */
	boost::uuids::uuid id;
	/**
	 * @brief The scene that owns the object
	 * 
	 */
	Scene* owner;

	/**
	 * @brief All components of the object
	 * 
	 */
	std::list<Component*> components;
public:
	/**
	 * @brief The name of the object
	 * 
	 */
	std::string name;
	/**
	 * @brief If the object is hidden from the scene
	 * 
	 * @todo This does nothing currently
	 * 
	 */
	bool isHidden;

	/**
	 * @brief The transform of the object
	 * 
	 */
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

