#include <DirectXMath.h>
#include "DSFXInput.h"
#include "CameraController.h"
#include "Object.hpp"


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
	const float horizontal = FXInput->GetAxis(LX, 0);
	const float vertical = FXInput->GetAxis(LY, 0);

	DirectX::XMVECTOR position = object->transform->GetLocalTranslation();
	position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(object->transform->Right(), deltaTime * horizontal * 2));
	position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(object->transform->Forward(), deltaTime * vertical * 2));
	object->transform->SetLocalTranslation(position);

	// Use left stick of joystick 0 to rotate camera
	const float rightHorizontal = FXInput->GetAxis(RX, 0);
	const float rightVertical = FXInput->GetAxis(RY, 0);

	DirectX::XMVECTOR rotation = object->transform->GetLocalRotation();
	DirectX::XMVECTOR rotationRightAxis = DirectX::XMQuaternionRotationAxis(object->transform->Right(), -deltaTime * rightVertical);
	DirectX::XMVECTOR rotationUpAxis = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), deltaTime * rightHorizontal);
	rotation = DirectX::XMQuaternionMultiply(rotation, rotationRightAxis);
	rotation = DirectX::XMQuaternionMultiply(rotation, rotationUpAxis);
	object->transform->SetLocalRotation(rotation);
}
