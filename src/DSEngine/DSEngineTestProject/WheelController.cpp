#include "WheelController.h"
#include <DirectXMath.h>
#include "Object.hpp"
#include "DSSInput.h"

WheelController::WheelController(Object * owner)
	: Component(owner)
{
}

WheelController::~WheelController()
{
}

void WheelController::Start()
{
}

void WheelController::Update(float deltaTime, float totalTime)
{
	float arrowHorizontal = SInput->GetAxis("ArrowHorizontal");
	float arrowVertical = SInput->GetAxis("ArrowVertical");
	object->GetComponent<WheelCollider>()->SetSteerFactor(arrowHorizontal);
	object->GetComponent<WheelCollider>()->SetMotorTorque(arrowVertical * 100.0f);

}
