#include <DirectXMath.h>
#include "MoveParentObject.h"
#include "Object.hpp"
#include "Scene.hpp"
#include "PBRMaterial.hpp"
#include <queue>
#include "DSSInput.h"


MoveParentObject::MoveParentObject(Object* owner)
	: Component(owner)
{
}


MoveParentObject::~MoveParentObject()
{
}

void MoveParentObject::Start()
{
	std::queue<Object*> objectQueue;

	objectQueue.push(object);

	while (!objectQueue.empty())
	{
		// Get the object
		Object* currentObject = objectQueue.front();

		// Process BFS
		objectQueue.pop();
		std::list<Transform*> children = currentObject->transform->GetChildren();
		for (Transform* child : children)
		{
			objectQueue.push(child->object);
		}

		// Process material
		std::list<MeshRenderer*> meshRendererComponents = currentObject->GetComponents<MeshRenderer>();
		for (MeshRenderer* meshRenderer : meshRendererComponents)
		{
			meshRenderers.push_back(meshRenderer);
		}
	}
}

void MoveParentObject::Update(float deltaTime, float totalTime)
{
	// Use Arrows/DPad to adjust the material
	float arrowHorizontal = SInput->GetAxis("ArrowHorizontal");
	float arrowVertical = SInput->GetAxis("ArrowVertical");


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
		/*DirectX::XMVECTOR position = object->transform->GetLocalTranslation();
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(-deltaTime, 0.0f, 0.0f, 0.0f));
		object->transform->SetLocalTranslation(position);*/
		object->GetComponent<RigidBody>()->AddForce(-2.0f, 0.0f, 0.0f);
	}

	if (FRawInput->GetKey(VK_RIGHT))
	{
		/*DirectX::XMVECTOR position = object->transform->GetLocalTranslation();
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(deltaTime, 0.0f, 0.0f, 0.0f));
		object->transform->SetLocalTranslation(position);*/
		object->GetComponent<RigidBody>()->AddForce(2.0f, 0.0f, 0.0f);
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

	// Use DPad to adjust the material

	for (MeshRenderer* renderer : meshRenderers)
	{
		PBRMaterial* material = static_cast<PBRMaterial*>(renderer->GetMaterial());

		material->parameters.metalness += 0.3f * deltaTime * arrowVertical;
		if (material->parameters.metalness > 1.0f)
			material->parameters.metalness = 1.0f;
		if (material->parameters.metalness < 0.0f)
			material->parameters.metalness = 0.0f;


		material->parameters.roughness -= 0.3f * deltaTime * arrowHorizontal;
		if (material->parameters.roughness > 1.0f)
			material->parameters.roughness = 1.0f;
		if (material->parameters.roughness < 0.0f)
			material->parameters.roughness = 0.0f;
	}

	float horizontal = SInput->GetAxis("MoveHorizontal");
	float vertical = SInput->GetAxis("MoveVertical");

	auto position = object->transform->GetLocalTranslation();
	position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(horizontal * 4 * deltaTime, 0.0f, 0.0f, 0.0f));
	position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(0.0f, vertical * 4 * deltaTime, 0.0f, 0.0f));
	object->transform->SetLocalTranslation(position);
}
