#include "WheelController.h"
#include <DirectXMath.h>
#include "Object.hpp"
#include "Scene.hpp"
#include "DSSInput.h"

WheelController::WheelController(Object * owner)
	: Component(owner)
{
	carRigidBody = nullptr;
}

WheelController::~WheelController()
{
}

void WheelController::Start()
{
}

void WheelController::Update(float deltaTime, float totalTime)
{
	if (carRigidBody == nullptr)
	{
		Object* car = object->GetScene()->FindObjectByName("AventHolder");
		carRigidBody = car->GetComponent<RigidBody>();
	}
	DirectX::XMFLOAT3 velocity = carRigidBody->GetVelocity();
	float RPM = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);

	RPM = (1 - RPM / 75.0f);
	float arrowHorizontal = SInput->GetAxis("Horizontal");
	float arrowVertical = SInput->GetAxis("Vertical");
	object->GetComponent<WheelCollider>()->SetSteerFactor(arrowHorizontal * RPM);
	object->GetComponent<WheelCollider>()->SetMotorTorque(arrowVertical * 4000.0f);
	if (SInput->GetButton("Brake")) {
		object->GetComponent<WheelCollider>()->SetBrakeTorque(4000.0f);
	}
}
