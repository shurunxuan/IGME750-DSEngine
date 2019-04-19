#include <DirectXMath.h>
#include "CameraController.h"
#include "Object.hpp"
#include "DSSInput.h"


CameraController::CameraController(Object* owner)
	: Component(owner)
{
}


CameraController::~CameraController()
{
}

void CameraController::Start()
{
	offset = DirectX::XMVectorSet(0.0f, 2.5f, -6.25f, 0.0f);
}

void CameraController::Update(float deltaTime, float totalTime)
{	
	// Use left stick of joystick 0 to move camera
	//float horizontal = SInput->GetAxis("Horizontal");
	//float vertical = SInput->GetAxis("Vertical");

	//DirectX::XMVECTOR position = object->transform->GetLocalTranslation();
	//position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(object->transform->Right(), deltaTime * horizontal * 20));
	//position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(object->transform->Forward(), deltaTime * vertical * 20));
	//object->transform->SetLocalTranslation(position);

	//// Use left stick of joystick 0 to rotate camera
	//float rightHorizontal = SInput->GetAxis("CameraHorizontal");
	//float rightVertical = SInput->GetAxis("CameraVertical");

	//DirectX::XMVECTOR rotation = object->transform->GetLocalRotation();
	//DirectX::XMVECTOR rotationRightAxis = DirectX::XMQuaternionRotationAxis(object->transform->Right(), -deltaTime * rightVertical);
	//DirectX::XMVECTOR rotationUpAxis = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), deltaTime * rightHorizontal);
	//rotation = DirectX::XMQuaternionMultiply(rotation, rotationRightAxis);
	//rotation = DirectX::XMQuaternionMultiply(rotation, rotationUpAxis);
	//object->transform->SetLocalRotation(rotation);

	if (following == nullptr) return;
	DirectX::XMVECTOR currentPosition = object->transform->GetGlobalTranslation();
	DirectX::XMVECTOR currentRotation = object->transform->GetGlobalRotation();
	DirectX::XMVECTOR desiredRotation = following->GetGlobalRotation();

	DirectX::XMVECTOR lerpRotation = DirectX::XMQuaternionSlerp(currentRotation, desiredRotation, 5.0f * deltaTime);
	DirectX::XMVECTOR rotateOffset = DirectX::XMVector4Transform(offset, DirectX::XMMatrixRotationQuaternion(lerpRotation));

	DirectX::XMVECTOR desiredPosition = DirectX::XMVectorAdd(following->GetGlobalTranslation(), rotateOffset);
	DirectX::XMVECTOR lerpPosition = DirectX::XMVectorLerp(currentPosition, desiredPosition, 20.0f * deltaTime);
	object->transform->SetLocalTranslation(lerpPosition);
	object->transform->SetLocalRotation(lerpRotation);
}
