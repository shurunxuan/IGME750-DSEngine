#pragma once
#include "Component.hpp"
#include "PBRMaterial.hpp"
#include "RefractionMaterial.h"
#include "MeshRenderer.hpp"
#include "Brick.h"


class ChangeBrickMaterial :
	public Component
{
public:
	ChangeBrickMaterial(Object* owner);
	~ChangeBrickMaterial();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

	SimpleVertexShader* refractionVS;
	SimplePixelShader* refractionPS;
	SimpleVertexShader* defaultVS;
	SimplePixelShader* defaultPS;
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	BrickColor color;

private:
	MeshRenderer* meshRenderer;

	PBRMaterial* material;
	RefractionMaterial* refMat;

	bool inRefractionMode;
};

