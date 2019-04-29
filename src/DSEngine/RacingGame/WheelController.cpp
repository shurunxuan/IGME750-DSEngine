#include "WheelController.h"
#include <DirectXMath.h>
#include "Object.hpp"
#include "Scene.hpp"
#include "DSSInput.h"

WheelController::WheelController(Object * owner)
	: Component(owner)
{
	carRigidBody = nullptr;
	carLightMaterial = nullptr;
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

	const DirectX::XMFLOAT3 velocity = carRigidBody->GetVelocity();
	const float speed = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z);
	const float steerLimit = 1 - speed / 75.0f;

	const bool stopped = speed < 0.1f;

	//float arrowHorizontal = SInput->GetAxis("Horizontal");
	float accel = SInput->GetAxis("Accelerate");
	float brake = SInput->GetAxis("Brake");
	if (carLightMaterial != nullptr)
	{
		if (brake > accel)
		{
			carLightMaterial->parameters.emission = { 1.0f, 0.0f, 0.0f, 2.0f };
		}
		else
		{
			carLightMaterial->parameters.emission = { 1.0f, 0.0f, 0.0f, 0.0f };
		}
	}
	const float steer = SInput->GetAxis("Horizontal");
	if (stopped)
	{
		reversing = brake > accel;
	}
	

	if (reversing)
	{
		const float temp = accel;
		accel = -brake * 0.2f;
		brake = temp;
	}

	SInput->SetControllerRumble(0, accel * speed / 75.0f, brake * speed / 75.0f);

	object->GetComponent<WheelCollider>()->SetSteerFactor(steer * steerLimit);
	object->GetComponent<WheelCollider>()->SetMotorTorque(accel * 4000.0f);
	object->GetComponent<WheelCollider>()->SetBrakeTorque(brake * 4000.0f);


}
