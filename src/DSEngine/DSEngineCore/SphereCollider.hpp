#pragma once
#pragma warning(disable:4251)
#include <DirectXCollision.h>
#include "Component.hpp"



class SphereCollider : public Component
{
public:
	explicit SphereCollider(Object* owner);

	~SphereCollider() final;

	void Start() override;

	void Update(float deltaTime, float totalTime) override final;

	DirectX::BoundingSphere AddSphereCollider();


	void SetColliderTransform(DirectX::FXMMATRIX M);

	void SetColliderTransform(float Scale, DirectX::FXMVECTOR Rotation, DirectX::FXMVECTOR Translation);

	DirectX::BoundingSphere* GetCollider();
private:
	DirectX::BoundingSphere collider;
};

inline SphereCollider::SphereCollider(Object * owner)
	:Component(owner)
{
	collider = DirectX::BoundingSphere();
}

inline SphereCollider::~SphereCollider()
{
}

inline void SphereCollider::Start()
{
}

inline void SphereCollider::Update(float deltaTime, float totalTime)
{
}

inline DirectX::BoundingSphere SphereCollider::AddSphereCollider()
{
	collider = DirectX::BoundingSphere();
	return collider;
}


inline void SphereCollider::SetColliderTransform(DirectX::FXMMATRIX M)
{
	collider.Transform(collider, M);
}

inline void SphereCollider::SetColliderTransform(float Scale, DirectX::FXMVECTOR Rotation, DirectX::FXMVECTOR Translation)
{
	collider.Transform(collider, Scale, Rotation, Translation);
}

inline DirectX::BoundingSphere* SphereCollider::GetCollider()
{
	return &collider;
}
