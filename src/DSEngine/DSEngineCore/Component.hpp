/**
 * @file Component.hpp
 * @author Victor Shu
 * @brief This file contains the class that represents
 * the component of the object
 * 
 * @version 0.1
 * @date 2019/03/09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once
#pragma warning(disable:4251)
#define BOOST_ALLOW_DEPRECATED_HEADERS
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>

class Object;


/**
 * @brief The component class that will be attached to an object
 * 
 */
class Component
{
public:
	/**
	 * @brief Construct a new Component object
	 * 
	 * @param owner The object that the component is attached to
	 */
	explicit Component(Object* owner);
	/**
	 * @brief Destroy the Component object
	 * 
	 */
	virtual ~Component();

	/**
	 * @brief Will be called as soon as the component is created
	 * 
	 */
	virtual void Start() = 0;
	/**
	 * @brief Will be called when the object updates
	 * 
	 * @param deltaTime The time that a frame costs
	 * @param totalTime The total time from the beginning of the application
	 */
	virtual void Update(float deltaTime, float totalTime) = 0;

	/**
	 * @brief Get the Instance ID
	 * 
	 * @return boost::uuids::uuid The UUID of the component
	 */
	boost::uuids::uuid GetInstanceId() const;

	/**
	 * @brief Indicates the component is active
	 * 
	 */
	bool isActive;

	/**
	 * @brief The object that the component is attached to
	 * 
	 */
	Object* object;


private:
	/**
	 * @brief The UUID of the component
	 * 
	 */
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


