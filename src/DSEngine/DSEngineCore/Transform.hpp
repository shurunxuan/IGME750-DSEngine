#pragma once
#pragma warning(disable:4251)

#include <DirectXMath.h>
#include <list>
#include "DSFLogging.h"
#include "Component.hpp"

class Transform : public Component
{
public:
	friend class Camera;
	explicit Transform(Object* owner);
	~Transform() final;

	void Start() override;
	void Update(float deltaTime, float totalTime) override final;

	void SetParent(Transform* newParent);
	Transform* GetParent() const;

	std::list<Transform*> GetChildren() const;

	DirectX::XMVECTOR GetLocalTranslation() const;
	DirectX::XMVECTOR GetLocalScale() const;
	DirectX::XMVECTOR GetLocalRotation() const;

	DirectX::XMVECTOR GetGlobalTranslation();
	DirectX::XMVECTOR GetGlobalScale();
	DirectX::XMVECTOR GetGlobalRotation();

	void SetLocalTranslation(float x, float y, float z);
	void SetLocalScale(float x, float y, float z);
	void SetLocalRotation(float x, float y, float z, float w);
	void SetLocalTranslation(DirectX::XMVECTOR transVec);
	void SetLocalScale(DirectX::XMVECTOR scaleVec);
	void SetLocalRotation(DirectX::XMVECTOR rot);

	DirectX::XMMATRIX GetLocalWorldMatrix();
	DirectX::XMMATRIX GetLocalInverseTransposeWorldMatrix();

	DirectX::XMMATRIX GetGlobalWorldMatrix();
	DirectX::XMMATRIX GetGlobalInverseTransposeWorldMatrix();

	DirectX::XMVECTOR Forward();
	DirectX::XMVECTOR Right();
	DirectX::XMVECTOR Up();

private:
	DirectX::XMVECTOR localTranslation;
	DirectX::XMVECTOR localScale;
	DirectX::XMVECTOR localRotation;

	DirectX::XMMATRIX localWorldMatrix;
	DirectX::XMMATRIX itLocalWorldMatrix;

	DirectX::XMMATRIX globalWorldMatrix;
	DirectX::XMMATRIX itGlobalWorldMatrix;

	bool shouldUpdate;

	void UpdateWorldMat();

	void ShouldUpdate();

	std::list<Transform*> children;
	Transform* parent;
};

inline Transform::Transform(Object* owner)
	: Component(owner)
{
	localTranslation = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	localScale = DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	localRotation = DirectX::XMQuaternionIdentity();

	parent = nullptr;

	shouldUpdate = true;

	UpdateWorldMat();
}

inline Transform::~Transform()
{
}

inline void Transform::Start()
{
}

inline void Transform::Update(float deltaTime, float totalTime)
{
	if (shouldUpdate)
		UpdateWorldMat();
}

inline void Transform::SetParent(Transform* newParent)
{
	UpdateWorldMat();

	if (parent != nullptr)
	{
		parent->children.remove(newParent);
	}

	DirectX::XMMatrixDecompose(&localScale, &localRotation, &localTranslation, XMMatrixTranspose(GetGlobalWorldMatrix()));
	parent = nullptr;
	UpdateWorldMat();

	parent = newParent;
	if (parent != nullptr)
	{
		const DirectX::XMMATRIX newLocal = DirectX::XMMatrixMultiply(XMMatrixTranspose(globalWorldMatrix), XMMatrixInverse(nullptr, XMMatrixTranspose(parent->GetGlobalWorldMatrix())));
		DirectX::XMMatrixDecompose(&localScale, &localRotation, &localTranslation, newLocal);
		UpdateWorldMat();
		parent->children.push_back(this);
	}
}

inline Transform* Transform::GetParent() const
{
	return parent;
}

inline std::list<Transform*> Transform::GetChildren() const
{
	return children;
}

inline DirectX::XMVECTOR Transform::GetLocalTranslation() const
{
	return localTranslation;
}

inline DirectX::XMVECTOR Transform::GetLocalScale() const
{
	return localScale;
}

inline DirectX::XMVECTOR Transform::GetLocalRotation() const
{
	return localRotation;
}

inline DirectX::XMVECTOR Transform::GetGlobalTranslation()
{
	DirectX::XMVECTOR translate;
	DirectX::XMVECTOR rotate;
	DirectX::XMVECTOR scale;

	const DirectX::XMMATRIX globalWorld = GetGlobalWorldMatrix();

	DirectX::XMMatrixDecompose(&scale, &rotate, &translate, XMMatrixTranspose(globalWorld));
	return translate;
}

inline DirectX::XMVECTOR Transform::GetGlobalScale()
{
	//DirectX::XMVECTOR scale = localScale;

	//if (parent != nullptr)
	//	scale = DirectX::XMVectorMultiply(scale, parent->GetGlobalScale());
	//
	//return scale;
	DirectX::XMVECTOR translate;
	DirectX::XMVECTOR rotate;
	DirectX::XMVECTOR scale;

	const DirectX::XMMATRIX globalWorld = GetGlobalWorldMatrix();

	DirectX::XMMatrixDecompose(&scale, &rotate, &translate, XMMatrixTranspose(globalWorld));
	return scale;
}

inline DirectX::XMVECTOR Transform::GetGlobalRotation()
{
	DirectX::XMVECTOR translate;
	DirectX::XMVECTOR rotate;
	DirectX::XMVECTOR scale;

	const DirectX::XMMATRIX globalWorld = GetGlobalWorldMatrix();

	DirectX::XMMatrixDecompose(&scale, &rotate, &translate, XMMatrixTranspose(globalWorld));
	return rotate;
}

inline void Transform::SetLocalTranslation(const float x, const float y, const float z)
{
	localTranslation = DirectX::XMVectorSet(x, y, z, 0.0f);
	ShouldUpdate();
}

inline void Transform::SetLocalScale(const float x, const float y, const float z)
{
	localScale = DirectX::XMVectorSet(x, y, z, 0.0f);
	if (x != y || x != z)
		LOG_WARNING << "Non-uniform scale will result in unexpected result!";
	ShouldUpdate();
}

inline void Transform::SetLocalRotation(float x, float y, float z, float w)
{
	localRotation = DirectX::XMVectorSet(x, y, z, w);
	ShouldUpdate();
}

inline void Transform::SetLocalTranslation(DirectX::XMVECTOR transVec)
{
	localTranslation = transVec;
	ShouldUpdate();
}

inline void Transform::SetLocalScale(DirectX::XMVECTOR scaleVec)
{
	localScale = scaleVec;
	if (DirectX::XMVectorGetByIndex(scaleVec, 0) != DirectX::XMVectorGetByIndex(scaleVec, 1) || DirectX::XMVectorGetByIndex(scaleVec, 0) != DirectX::XMVectorGetByIndex(scaleVec, 2))
		LOG_WARNING << "Non-uniform scale will result in unexpected result!";
	ShouldUpdate();
}

inline void Transform::SetLocalRotation(DirectX::XMVECTOR rot)
{
	localRotation = rot; 
	ShouldUpdate();
}

inline DirectX::XMMATRIX Transform::GetLocalWorldMatrix()
{
	if (shouldUpdate) UpdateWorldMat();
	return localWorldMatrix;
}

inline DirectX::XMMATRIX Transform::GetLocalInverseTransposeWorldMatrix()
{
	if (shouldUpdate) UpdateWorldMat();
	return itLocalWorldMatrix;
}

inline DirectX::XMMATRIX Transform::GetGlobalWorldMatrix()
{
	if (shouldUpdate) UpdateWorldMat();
	return globalWorldMatrix;
}

inline DirectX::XMMATRIX Transform::GetGlobalInverseTransposeWorldMatrix()
{
	if (shouldUpdate) UpdateWorldMat();
	return itGlobalWorldMatrix;
}

inline DirectX::XMVECTOR Transform::Forward()
{
	const DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const DirectX::XMVECTOR result = DirectX::XMVector4Transform(forward, XMMatrixTranspose(GetGlobalWorldMatrix()));

	return result;
}

inline DirectX::XMVECTOR Transform::Right()
{
	const DirectX::XMVECTOR right = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR result = DirectX::XMVector4Transform(right, XMMatrixTranspose(GetGlobalWorldMatrix()));

	return result;
}

inline DirectX::XMVECTOR Transform::Up()
{
	const DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR result = DirectX::XMVector4Transform(up, XMMatrixTranspose(GetGlobalWorldMatrix()));

	return result;
}


inline void Transform::UpdateWorldMat()
{
	const DirectX::XMMATRIX t = DirectX::XMMatrixTranslationFromVector(localTranslation);
	const DirectX::XMMATRIX r = DirectX::XMMatrixRotationQuaternion(localRotation);
	const DirectX::XMMATRIX s = DirectX::XMMatrixScalingFromVector(localScale);

	const DirectX::XMMATRIX w = s * r * t;
	localWorldMatrix = DirectX::XMMatrixTranspose(w);
	itLocalWorldMatrix = DirectX::XMMatrixInverse(nullptr, w);

	globalWorldMatrix = w;

	if (parent != nullptr)
	{
		globalWorldMatrix = DirectX::XMMatrixMultiply(globalWorldMatrix, XMMatrixTranspose(parent->GetGlobalWorldMatrix()));
	}

	itGlobalWorldMatrix = DirectX::XMMatrixInverse(nullptr, globalWorldMatrix);
	globalWorldMatrix = XMMatrixTranspose(globalWorldMatrix);

	shouldUpdate = false;
}

inline void Transform::ShouldUpdate()
{
	for (Transform* child : children)
	{
		child->ShouldUpdate();
	}

	shouldUpdate = true;
}

