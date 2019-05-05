#include "PostProcessingController.h"
#include "DSSInput.h"
PostProcessingController::PostProcessingController(Object* owner)
	:Component(owner)
{
}

PostProcessingController::~PostProcessingController()
{
}

void PostProcessingController::Start()
{
}

void PostProcessingController::Update(float deltaTime, float totalTime)
{
	int* sourceIndices = nullptr;
	darkCorner->GetSourceIndices(&sourceIndices);

	if (SInput->GetButtonDown("ToggleDarkCorner"))
		darkCorner->parameters.intensity = 1.0f - darkCorner->parameters.intensity;

	if (SInput->GetButtonDown("ToggleIL"))
		ssao->indirectLighting = !ssao->indirectLighting;

	if (SInput->GetButtonDown("TurnOffPP"))
		sourceIndices[0] = 7;

	if (SInput->GetButtonDown("TurnOnBloom"))
		sourceIndices[0] = 6;

	if (SInput->GetButtonDown("TurnOnSSAO"))
		sourceIndices[0] = 3;
}
