#include "ChangeBrickMaterial.h"
#include "Object.hpp"
#include "DSSInput.h"


ChangeBrickMaterial::ChangeBrickMaterial(Object* owner)
	: Component(owner)
{
	material = nullptr;
	refMat = nullptr;

	inRefractionMode = false;

	refractionVS = nullptr;
	refractionPS = nullptr;
}


ChangeBrickMaterial::~ChangeBrickMaterial()
{
	delete refractionVS;
	delete refractionPS;
}

void ChangeBrickMaterial::Start()
{
	meshRenderer = object->transform->GetChildAt(0)->object->GetComponent<MeshRenderer>();
	material = static_cast<PBRMaterial*>(meshRenderer->GetMaterial());
}

void ChangeBrickMaterial::Update(float deltaTime, float totalTime)
{
	if (refractionVS == nullptr)
	{
		refractionVS = new SimpleVertexShader(device, context);
		refractionVS->LoadShaderFile(L"RefractionVertexShader.cso");
	}
	if (refractionPS == nullptr)
	{
		refractionPS = new SimplePixelShader(device, context);
		refractionPS->LoadShaderFile(L"RefractionShader.cso");
	}
	float arrowHorizontal = SInput->GetAxis("ArrowHorizontal");
	float arrowVertical = SInput->GetAxis("ArrowVertical");

	if (SInput->GetButtonDown("ToggleRefraction"))
	{
		inRefractionMode = !inRefractionMode;
		if (color == YellowBrick)
		{
			if (inRefractionMode)
			{
				PBRMaterial* currentMaterial = static_cast<PBRMaterial*>(meshRenderer->GetMaterial());
				std::shared_ptr<RefractionMaterial> refractionMaterial = std::make_shared<RefractionMaterial>(refractionVS, refractionPS, device, context);
				refractionMaterial->transparent = true;
				refractionMaterial->grab = true;

				refractionMaterial->LoadNormalTexture("Assets/Textures/wall.tif");
				refractionMaterial->parameters = currentMaterial->parameters;

				meshRenderer->SetMaterial(refractionMaterial);
			}
			else
			{
				RefractionMaterial* currentMaterial = static_cast<RefractionMaterial*>(meshRenderer->GetMaterial());
				std::shared_ptr<PBRMaterial> pbrMaterial = std::make_shared<RefractionMaterial>(defaultVS, defaultPS, device, context);
				pbrMaterial->transparent = true;
				pbrMaterial->grab = true;

				pbrMaterial->LoadNormalTexture("Assets/Textures/wall.tif");
				pbrMaterial->parameters = currentMaterial->parameters;

				meshRenderer->SetMaterial(pbrMaterial);
			}
		}
	}

	material = static_cast<PBRMaterial*>(meshRenderer->GetMaterial());

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

	if (SInput->GetButtonDown("ToggleNormalMap"))
	{
		material->parameters.hasNormalMap = 1 - material->parameters.hasNormalMap;
	}
}
