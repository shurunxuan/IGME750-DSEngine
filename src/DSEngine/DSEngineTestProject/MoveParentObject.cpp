#include <DirectXMath.h>
#include "MoveParentObject.h"
#include "Object.hpp"
#include "Scene.hpp"
#include "DSFRawInput.h"
#include "DSFXInput.h"


MoveParentObject::MoveParentObject(Object* owner)
	: Component(owner)
{
}


MoveParentObject::~MoveParentObject()
{
}

void MoveParentObject::Start()
{
}

void MoveParentObject::Update(float deltaTime, float totalTime)
{

	// Use arrow keys to move object
	if (FRawInput->GetKey(VK_UP))
	{
		DirectX::XMVECTOR position = object->transform->GetLocalTranslation();
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(0.0f, deltaTime, 0.0f, 0.0f));
		object->transform->SetLocalTranslation(position);
	}

	if (FRawInput->GetKey(VK_DOWN))
	{
		DirectX::XMVECTOR position = object->transform->GetLocalTranslation();
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(0.0f, -deltaTime, 0.0f, 0.0f));
		object->transform->SetLocalTranslation(position);
	}

	if (FRawInput->GetKey(VK_LEFT))
	{
		DirectX::XMVECTOR position = object->transform->GetLocalTranslation();
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(-deltaTime, 0.0f, 0.0f, 0.0f));
		object->transform->SetLocalTranslation(position);
	}

	if (FRawInput->GetKey(VK_RIGHT))
	{
		DirectX::XMVECTOR position = object->transform->GetLocalTranslation();
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(deltaTime, 0.0f, 0.0f, 0.0f));
		object->transform->SetLocalTranslation(position);
	}

	// Use numpad numbers to rotate object
	if (FRawInput->GetKey(VK_NUMPAD8))
	{
		DirectX::XMVECTOR rotation = object->transform->GetLocalRotation();
		DirectX::XMVECTOR rotationRightAxis = DirectX::XMQuaternionRotationAxis(object->transform->Right(), deltaTime);
		rotation = DirectX::XMQuaternionMultiply(rotation, rotationRightAxis);
		object->transform->SetLocalRotation(rotation);
	}

	if (FRawInput->GetKey(VK_NUMPAD2))
	{
		DirectX::XMVECTOR rotation = object->transform->GetLocalRotation();
		DirectX::XMVECTOR rotationRightAxis = DirectX::XMQuaternionRotationAxis(object->transform->Right(), -deltaTime);
		rotation = DirectX::XMQuaternionMultiply(rotation, rotationRightAxis);
		object->transform->SetLocalRotation(rotation);
	}

	if (FRawInput->GetKey(VK_NUMPAD4))
	{
		DirectX::XMVECTOR rotation = object->transform->GetLocalRotation();
		DirectX::XMVECTOR rotationUpAxis = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), deltaTime);
		rotation = DirectX::XMQuaternionMultiply(rotation, rotationUpAxis);
		object->transform->SetLocalRotation(rotation);
	}

	if (FRawInput->GetKey(VK_NUMPAD6))
	{
		DirectX::XMVECTOR rotation = object->transform->GetLocalRotation();
		DirectX::XMVECTOR rotationUpAxis = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), -deltaTime);
		rotation = DirectX::XMQuaternionMultiply(rotation, rotationUpAxis);
		object->transform->SetLocalRotation(rotation);

	}

	// Use 1 to scale up, 2 to scale down
	if (FRawInput->GetKey('1'))
	{
		DirectX::XMVECTOR scale = object->transform->GetLocalScale();
		scale = DirectX::XMVectorScale(scale, 1 + deltaTime);
		object->transform->SetLocalScale(scale);
	}
	if (FRawInput->GetKey('2'))
	{
		DirectX::XMVECTOR scale = object->transform->GetLocalScale();
		scale = DirectX::XMVectorScale(scale, 1 / (1 + deltaTime));
		object->transform->SetLocalScale(scale);
	}
}
