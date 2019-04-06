#include "ChangeBrickMaterial.h"
#include "Object.hpp"
#include "DSSInput.h"
#include "MeshRenderer.hpp"


ChangeBrickMaterial::ChangeBrickMaterial(Object* owner)
	: Component(owner)
{
	material = nullptr;
}


ChangeBrickMaterial::~ChangeBrickMaterial()
{
}

void ChangeBrickMaterial::Start()
{
	MeshRenderer* meshRenderer = object->transform->GetChildAt(0)->object->GetComponent<MeshRenderer>();
	material = static_cast<PBRMaterial*>(meshRenderer->GetMaterial());
}

void ChangeBrickMaterial::Update(float deltaTime, float totalTime)
{
	float arrowHorizontal = SInput->GetAxis("ArrowHorizontal");
	float arrowVertical = SInput->GetAxis("ArrowVertical");


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
