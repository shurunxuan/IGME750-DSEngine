#include "TestTransform.h"

#include <DirectXMath.h>
#include "DSFRawInput.h"
#include "Object.hpp"


TestTransform::TestTransform(Object* owner)
	: Component(owner)
{
}


TestTransform::~TestTransform()
{
}

void TestTransform::Start()
{
	gotParent = false;
}

void TestTransform::Update(float deltaTime, float totalTime)
{
	if (parent == nullptr && !gotParent)
	{
		parent = object->transform->GetParent();
		if (parent != nullptr)
			gotParent = true;
	}
	// Use WASD to move object
	if (FRawInput->GetKey('W'))
	{
		DirectX::XMVECTOR position = object->transform->GetLocalTranslation();
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(0.0f, deltaTime, 0.0f, 0.0f));
		object->transform->SetLocalTranslation(position);
	}

	if (FRawInput->GetKey('S'))
	{
		DirectX::XMVECTOR position = object->transform->GetLocalTranslation();
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(0.0f, -deltaTime, 0.0f, 0.0f));
		object->transform->SetLocalTranslation(position);
	}

	if (FRawInput->GetKey('A'))
	{
		DirectX::XMVECTOR position = object->transform->GetLocalTranslation();
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(-deltaTime, 0.0f, 0.0f, 0.0f));
		object->transform->SetLocalTranslation(position);
	}

	if (FRawInput->GetKey('D'))
	{
		DirectX::XMVECTOR position = object->transform->GetLocalTranslation();
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(deltaTime, 0.0f, 0.0f, 0.0f));
		object->transform->SetLocalTranslation(position);
	}


	// Use 9 to scale up, 0 to scale down
	if (FRawInput->GetKey('9'))
	{
		DirectX::XMVECTOR scale = object->transform->GetLocalScale();
		scale = DirectX::XMVectorScale(scale, 1 + deltaTime);
		object->transform->SetLocalScale(scale);
	}
	if (FRawInput->GetKey('0'))
	{
		DirectX::XMVECTOR scale = object->transform->GetLocalScale();
		scale = DirectX::XMVectorScale(scale, 1 / (1 + deltaTime));
		object->transform->SetLocalScale(scale);
	}
	// Use ctrl to toggle transform parent
	if (FRawInput->GetKeyDown(VK_CONTROL))
	{
		if (object->transform->GetParent() == nullptr)
			object->transform->SetParent(parent);
		else
			object->transform->SetParent(nullptr);
	}
}


