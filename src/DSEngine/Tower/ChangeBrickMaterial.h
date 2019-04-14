#pragma once
#include "Component.hpp"
#include "PBRMaterial.hpp"


class ChangeBrickMaterial :
	public Component
{
public:
	ChangeBrickMaterial(Object* owner);
	~ChangeBrickMaterial();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

private:
	PBRMaterial* material;
};

