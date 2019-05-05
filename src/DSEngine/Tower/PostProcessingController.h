#pragma once
#include "Component.hpp"
#include "PPDarkCornerMaterial.h"
#include "SSAOMaterial.h"

class PostProcessingController :
	public Component
{
public:
	PostProcessingController(Object* owner);
	~PostProcessingController();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

	PPDarkCornerMaterial* darkCorner;
	PostProcessingMaterial* applySSAO;
	SSAOMaterial* ssao;
};

