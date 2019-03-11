#pragma once
#pragma warning(disable:4251)
#define BOOST_ALLOW_DEPRECATED_HEADERS
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

class Object;

class Component
{
public:
	explicit Component(Object* owner);
	virtual ~Component();

	virtual void Start() = 0;
	virtual void Update(float deltaTime, float totalTime) = 0;

	boost::uuids::uuid GetInstanceId() const;

	bool isActive;
	Object* object;


private:
	boost::uuids::uuid id;
};

inline Component::Component(Object* owner)
	: isActive(true)
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


