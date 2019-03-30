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
