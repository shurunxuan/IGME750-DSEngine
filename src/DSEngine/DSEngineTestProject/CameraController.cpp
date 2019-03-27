#include <DirectXMath.h>
#include "DSFXInput.h"
#include "CameraController.h"
#include "Object.hpp"
#include "DSFRawInput.h"


CameraController::CameraController(Object* owner)
	: Component(owner)
{
}


CameraController::~CameraController()
{
}

void CameraController::Start()
{
}

void CameraController::Update(float deltaTime, float totalTime)
{	
	// Use left stick of joystick 0 to move camera
	float horizontal = FXInput->GetAxis(LX, 0);
	float vertical = FXInput->GetAxis(LY, 0);

	if (FRawInput->GetKey('W'))
	{
		vertical += 1;
	}

	if (FRawInput->GetKey('S'))
	{
		vertical -= 1;
	}

	if (FRawInput->GetKey('A'))
	{
		horizontal -= 1;
	}

	if (FRawInput->GetKey('D'))
	{
		horizontal += 1;
	}


	DirectX::XMVECTOR position = object->transform->GetLocalTranslation();
	position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(object->transform->Right(), deltaTime * horizontal * 2));
	position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(object->transform->Forward(), deltaTime * vertical * 2));
	object->transform->SetLocalTranslation(position);

	// Use left stick of joystick 0 to rotate camera
	float rightHorizontal = FXInput->GetAxis(RX, 0);
	float rightVertical = FXInput->GetAxis(RY, 0);

	if (FRawInput->GetKey('I'))
	{
		rightVertical += 1;
	}

	if (FRawInput->GetKey('K'))
	{
		rightVertical -= 1;
	}

	if (FRawInput->GetKey('J'))
	{
		rightHorizontal -= 1;
	}

	if (FRawInput->GetKey('L'))
	{
		rightHorizontal += 1;
	}

	DirectX::XMVECTOR rotation = object->transform->GetLocalRotation();
	DirectX::XMVECTOR rotationRightAxis = DirectX::XMQuaternionRotationAxis(object->transform->Right(), -deltaTime * rightVertical);
	DirectX::XMVECTOR rotationUpAxis = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), deltaTime * rightHorizontal);
	rotation = DirectX::XMQuaternionMultiply(rotation, rotationRightAxis);
	rotation = DirectX::XMQuaternionMultiply(rotation, rotationUpAxis);
	object->transform->SetLocalRotation(rotation);
}
