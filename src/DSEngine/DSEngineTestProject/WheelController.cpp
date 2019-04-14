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

	// Use arrow keys to move object
	if (FRawInput->GetKey(VK_UP))
	{
		object->GetComponent<WheelCollider>()->SetMotorTorque(40.0f);
	}

	if (FRawInput->GetKey(VK_DOWN))
	{
		object->GetComponent<WheelCollider>()->SetBrakeTorque(-20.0f);
	}

}
