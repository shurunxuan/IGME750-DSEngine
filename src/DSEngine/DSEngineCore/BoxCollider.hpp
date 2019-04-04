/**
 * @file BoxCollider.hpp
 * @author Weihao Yan
 * @brief This file contains the class that represents
 * the BoxCollider
 *
 * @version 0.1
 * @date 2019/04/03
 *
 * @copyright Copyright (c) 2019
 *
 */

#pragma once
#pragma warning(disable:4251)
#include <DirectXCollision.h>
#include "Component.hpp"



class BoxCollider : public Component
{
public:

	/**
	 * @brief Construct a new BoxCollider
	 *
	 * @param owner The object that owns the BoxCollider
	 */
	explicit BoxCollider(Object* owner);

	/**
	 * @brief Destroy the BoxCollider object
	 *
	 */
	~BoxCollider();

	/**
	 * @brief Does nothing
	 *
	 */
	void Start() override;

	/**
	 * @brief Does nothing
	 *
	 */
	void Update(float deltaTime, float totalTime) override;

	/**
	 * @brief Get the BoundingBox* of this BoxCollider
	 *
	 */
	DirectX::BoundingBox* GetCollider();

private:
	/**
	 * @brief The BoundingBox of this BoxCollider
	 *
	 */
	DirectX::BoundingBox collider;
};

inline BoxCollider::BoxCollider(Object* owner)
	:Component(owner)
{
	collider = DirectX::BoundingBox();
}

inline BoxCollider::~BoxCollider()
{
}

inline void BoxCollider::Start()
{
}

inline void BoxCollider::Update(float deltaTime, float totalTime)
{
}

inline DirectX::BoundingBox* BoxCollider::GetCollider()
{
	return &collider;
}
