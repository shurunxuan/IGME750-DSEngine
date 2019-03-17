#pragma once
#pragma warning(disable:4251)

#include <DirectXMath.h>
#include <list>
#include "DSFLogging.h"
#include "Component.hpp"

/**
 * @brief The transform of the object
 * 
 */
class Transform : public Component
{
public:
	/// @cond FRIEND_CLASS_DECLARATION
	friend class Camera;
	/// @endcond
	
	/**
	 * @brief Construct a new Transform
	 * 
	 * @param owner The object that owns the Transform
	 */
	explicit Transform(Object* owner);
	/**
	 * @brief Destroy the Transform object
	 * 
	 */
	~Transform() final;

	/**
	 * @brief Does nothing
	 * 
	 */
	void Start() override;
	/**
	 * @brief If required, update the vectors and matrices of the transform
	 * 
	 * @param deltaTime The time a frame costs
	 * @param totalTime The total time from the beginning of the application
	 */
	void Update(float deltaTime, float totalTime) override final;

	/**
	 * @brief Set a new parent
	 * 
	 * @param newParent The new parent, can be nullptr to remove the parent
	 */
	void SetParent(Transform* newParent);
	/**
	 * @brief Get the parent Transform
	 * 
	 * @return Transform* The pointer of the parent transform, nullptr if there's no parent
	 */
	Transform* GetParent() const;

	/**
	 * @brief Get all children of the transform
	 * 
	 * @return std::list<Transform*> All children of the transform
	 * 
	 * @todo Consider add a function that get a specified child with a property
	 */
	std::list<Transform*> GetChildren() const;

	/**
	 * @brief Get the Local Translation
	 * 
	 * @return DirectX::XMVECTOR The local translation
	 */
	DirectX::XMVECTOR GetLocalTranslation() const;
	/**
	 * @brief Get the Local Scale 
	 * 
	 * @return DirectX::XMVECTOR The local scale
	 */
	DirectX::XMVECTOR GetLocalScale() const;
	/**
	 * @brief Get the Local Rotation
	 * 
	 * @return DirectX::XMVECTOR The local rotation
	 */
	DirectX::XMVECTOR GetLocalRotation() const;

	/**
	 * @brief Get the Global Translation 
	 * 
	 * @return DirectX::XMVECTOR The global translation
	 */
	DirectX::XMVECTOR GetGlobalTranslation();
	/**
	 * @brief Get the Global Scale
	 * 
	 * @return DirectX::XMVECTOR The global scale
	 */
	DirectX::XMVECTOR GetGlobalScale();
	/**
	 * @brief Get the Global Rotation
	 * 
	 * @return DirectX::XMVECTOR The global rotation
	 */
	DirectX::XMVECTOR GetGlobalRotation();

	/**
	 * @brief Set the Local Translation
	 * 
	 * @param x The translation along the x axis
	 * @param y The translation along the y axis
	 * @param z The translation along the z axis
	 */
	void SetLocalTranslation(float x, float y, float z);
	/**
	 * @brief Set the Local Scale
	 * 
	 * WARNING: Using non-uniform scale may cause some unexpected behavior!
	 * 
	 * @param x The scale of the x axis
	 * @param y The scale of the y axis
	 * @param z The scale of the z axis
	 */
	void SetLocalScale(float x, float y, float z);
	/**
	 * @brief Set the Local Rotation
	 * 
	 * @param x The x component of the new rotation quaternion
	 * @param y The y component of the new rotation quaternion
	 * @param z The z component of the new rotation quaternion
	 * @param w The w component of the new rotation quaternion
	 */
	void SetLocalRotation(float x, float y, float z, float w);
	/**
	 * @brief Set the Local Translation
	 * 
	 * @param transVec The new local translation vector
	 */
	void SetLocalTranslation(DirectX::XMVECTOR transVec);
	/**
	 * @brief Set the Local Scale 
	 * 
	 * WARNING: Using non-uniform scale may cause some unexpected behavior!
	 * 
	 * @param scaleVec The new local scale vector
	 */
	void SetLocalScale(DirectX::XMVECTOR scaleVec);
	/**
	 * @brief Set the Local Rotation
	 * 
	 * @param rot The new local rotation quaternion
	 */
	void SetLocalRotation(DirectX::XMVECTOR rot);

	/**
	 * @brief Get the Local World Matrix
	 * 
	 * @return DirectX::XMMATRIX The local world matrix
	 */
	DirectX::XMMATRIX GetLocalWorldMatrix();
	/**
	 * @brief Get the Local Inverse Transpose World Matrix
	 * 
	 * @return DirectX::XMMATRIX The local inverse transpose world matrix
	 */
	DirectX::XMMATRIX GetLocalInverseTransposeWorldMatrix();

	/**
	 * @brief Get the Global World Matrix
	 * 
	 * @return DirectX::XMMATRIX The global world matrix
	 */
	DirectX::XMMATRIX GetGlobalWorldMatrix();
	/**
	 * @brief Get the Global Inverse Transpose World Matrix
	 * 
	 * @return DirectX::XMMATRIX The global inverse transpose world matrix
	 */
	DirectX::XMMATRIX GetGlobalInverseTransposeWorldMatrix();

	/**
	 * @brief Get the Global Forward unit vector of the transform
	 * 
	 * @return DirectX::XMVECTOR The Global Forward vector
	 */
	DirectX::XMVECTOR Forward();
	/**
	 * @brief Get the Global Right unit vector of the transform
	 * 
	 * @return DirectX::XMVECTOR The Global Right vector
	 */
	DirectX::XMVECTOR Right();
	/**
	 * @brief Get the Global Up unit vector of the transform
	 * 
	 * @return DirectX::XMVECTOR The Global Up vector
	 */
	DirectX::XMVECTOR Up();

private:
	/**
	 * @brief The local translation
	 * 
	 */
	DirectX::XMVECTOR localTranslation;
	/**
	 * @brief The local scale
	 * 
	 */
	DirectX::XMVECTOR localScale;
	/**
	 * @brief The local rotation
	 * 
	 */
	DirectX::XMVECTOR localRotation;

	/**
	 * @brief The local world matrix
	 * 
	 */
	DirectX::XMMATRIX localWorldMatrix;
	/**
	 * @brief The local inverse transpose world matrix
	 * 
	 */
	DirectX::XMMATRIX itLocalWorldMatrix;

	/**
	 * @brief The global world matrix
	 * 
	 */
	DirectX::XMMATRIX globalWorldMatrix;
	/**
	 * @brief The global inverse transpose world matrix
	 * 
	 */
	DirectX::XMMATRIX itGlobalWorldMatrix;

	/**
	 * @brief Indicates if the vectors/matrices should be updated
	 * 
	 */
	bool shouldUpdate;

	/**
	 * @brief Update the world matrices
	 * 
	 */
	void UpdateWorldMat();

	/**
	 * @brief Request the update of this transform and all children transforms recursively
	 * 
	 */
	void ShouldUpdate();

	/**
	 * @brief All children of the transform
	 * 
	 */
	std::list<Transform*> children;
	/**
	 * @brief The parent of the transform, nullptr if the transform is the root
	 * 
	 */
	Transform* parent;
};

/// @cond INLINE_DEFINITION
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
/// @endcond
