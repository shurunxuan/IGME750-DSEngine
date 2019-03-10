#pragma once
#pragma warning(disable:4251)

#include "Object.hpp"

class Object;

class Component
{
public:
	explicit Component(Object* owner);
	virtual ~Component();

	virtual void Update(float deltaTime) = 0;

	boost::uuids::uuid GetInstanceId() const;

	bool isActive;
	Object* object;


private:
	boost::uuids::uuid id;
};

inline Component::Component(Object* owner)
	: isActive(false)
	, object(owner)
	, id(boost::uuids::random_generator()())
{
}

inline Component::~Component()
{
}

inline boost::uuids::uuid Component::GetInstanceId() const
{
	return id;
}


