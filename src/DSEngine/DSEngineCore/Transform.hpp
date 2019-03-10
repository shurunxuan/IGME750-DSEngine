#pragma once
#pragma warning(disable:4251)

#include <DirectXMath.h>
#include "Component.hpp"

class Transform : public Component
{
public:
	explicit Transform(Object* owner);
	~Transform() final;

	void Start() override;
	void Update(float deltaTime) override final;

	DirectX::XMVECTOR GetTranslation() const;
	DirectX::XMVECTOR GetScale() const;
	DirectX::XMVECTOR GetRotation() const;

	void SetTranslation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetRotation(float x, float y, float z, float w);
	void SetTranslation(DirectX::XMVECTOR transVec);
	void SetScale(DirectX::XMVECTOR scaleVec);
	void SetRotation(DirectX::XMVECTOR rot);

	DirectX::XMMATRIX GetWorldMatrix() const;
	DirectX::XMMATRIX GetInverseTransposeWorldMatrix() const;

	DirectX::XMVECTOR Forward() const;
	DirectX::XMVECTOR Right() const;
	DirectX::XMVECTOR Up() const;

private:
	DirectX::XMVECTOR translation;
	DirectX::XMVECTOR scale;
	DirectX::XMVECTOR rotation;

	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX itWorldMatrix;

	bool shouldUpdate;

	void UpdateWorldMat();
};

inline Transform::Transform(Object* owner)
	: Component(owner)
{
	translation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	scale = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	rotation = DirectX::XMQuaternionIdentity();

	shouldUpdate = true;

	UpdateWorldMat();
}

inline Transform::~Transform()
{
}

inline void Transform::Start()
{
}

inline void Transform::Update(float deltaTime)
{
	if (shouldUpdate)
		UpdateWorldMat();
}

inline DirectX::XMVECTOR Transform::GetTranslation() const
{
	return translation;
}

inline DirectX::XMVECTOR Transform::GetScale() const
{
	return scale;
}

inline DirectX::XMVECTOR Transform::GetRotation() const
{
	return rotation;
}

inline void Transform::SetTranslation(const float x, const float y, const float z)
{
	translation = DirectX::XMVectorSet(x, y, z, 0.0f);
	shouldUpdate = true;
}

inline void Transform::SetScale(const float x, const float y, const float z)
{
	scale = DirectX::XMVectorSet(x, y, z, 0.0f);
	shouldUpdate = true;
}

inline void Transform::SetRotation(float x, float y, float z, float w)
{
	rotation = DirectX::XMVectorSet(x, y, z, w);
	shouldUpdate = true;
}

inline void Transform::SetTranslation(DirectX::XMVECTOR transVec)
{
	translation = transVec;
	shouldUpdate = true;
}

inline void Transform::SetScale(DirectX::XMVECTOR scaleVec)
{
	scale = scaleVec;
	shouldUpdate = true;
}

inline void Transform::SetRotation(DirectX::XMVECTOR rot)
{
	rotation = rot;
	shouldUpdate = true;
}

inline DirectX::XMMATRIX Transform::GetWorldMatrix() const
{
	return worldMatrix;
}

inline DirectX::XMMATRIX Transform::GetInverseTransposeWorldMatrix() const
{
	return itWorldMatrix;
}

inline DirectX::XMVECTOR Transform::Forward() const
{
	const DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const DirectX::XMVECTOR result = DirectX::XMVector4Transform(forward, XMMatrixTranspose(worldMatrix));

	return result;
}

inline DirectX::XMVECTOR Transform::Right() const
{
	const DirectX::XMVECTOR right = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR result = DirectX::XMVector4Transform(right, XMMatrixTranspose(worldMatrix));

	return result;
}

inline DirectX::XMVECTOR Transform::Up() const
{
	const DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR result = DirectX::XMVector4Transform(up, XMMatrixTranspose(worldMatrix));

	return result;
}


inline void Transform::UpdateWorldMat()
{
	const DirectX::XMMATRIX t = DirectX::XMMatrixTranslationFromVector(translation);
	const DirectX::XMMATRIX r = DirectX::XMMatrixRotationQuaternion(rotation);
	const DirectX::XMMATRIX s = DirectX::XMMatrixScalingFromVector(scale);

	const DirectX::XMMATRIX w = s * r * t;
	worldMatrix = DirectX::XMMatrixTranspose(w);
	itWorldMatrix = DirectX::XMMatrixInverse(nullptr, w);

	shouldUpdate = false;
}

