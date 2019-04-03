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

using namespace DirectX;

class BoxCollider : public Component
{
public:
	explicit BoxCollider(Object* owner);
	~BoxCollider();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;
	BoundingBox* GetCollider();

private:
	BoundingBox collider;
};

inline BoxCollider::BoxCollider(Object* owner)
	:Component(owner)
{
	collider = BoundingBox();
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

inline BoundingBox* BoxCollider::GetCollider()
{
	return &collider;
}
