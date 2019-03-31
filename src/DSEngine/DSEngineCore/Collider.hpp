#pragma once
#pragma warning(disable:4251)
#include <DirectXCollision.h>
#include "Component.hpp"

using namespace DirectX;

class Collider : public Component
{
public:
	explicit Collider(Object* owner);

	~Collider() final;

	void Start() override;

	void Update(float deltaTime, float totalTime) override final;

	BoundingSphere AddSphereCollider();

	BoundingBox AddBoxCollider();

	void SetColliderTransform(FXMMATRIX M);

	void SetColliderTransform(float Scale, FXMVECTOR Rotation, FXMVECTOR Translation);

	BoundingSphere* GetCollider();
private:
	BoundingSphere collider;
	//BoundingBox colliderBox;
};

inline Collider::Collider(Object * owner)
	:Component(owner)
{
	collider = BoundingSphere();
}

inline Collider::~Collider()
{
}

inline void Collider::Start()
{
	/*collider = BoundingSphere();*/
}

inline void Collider::Update(float deltaTime, float totalTime)
{
}

inline DirectX::BoundingSphere Collider::AddSphereCollider()
{
	collider = BoundingSphere();
	return collider;
}

//inline BoundingBox Collider::AddBoxCollider()
//{
//	colliderBox = BoundingBox();
//	return colliderBox;
//}

inline void Collider::SetColliderTransform(FXMMATRIX M)
{
	collider.Transform(collider, M);
}

inline void Collider::SetColliderTransform(float Scale, FXMVECTOR Rotation, FXMVECTOR Translation)
{
	collider.Transform(collider, Scale, Rotation, Translation);
}

inline BoundingSphere* Collider::GetCollider()
{
	return &collider;
}
