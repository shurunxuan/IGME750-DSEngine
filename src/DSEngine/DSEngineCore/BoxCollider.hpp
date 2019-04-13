#pragma once
#pragma warning(disable:4251)
#include <DirectXCollision.h>
#include "Component.hpp"


class BoxCollider : public Component
{
public:
	explicit BoxCollider(Object* owner);
	~BoxCollider();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;
	DirectX::BoundingBox* GetCollider();

private:
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
