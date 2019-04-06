#include <DirectXMath.h>
#include "FreeCam.h"
#include "Object.hpp"
#include "DSSInput.h"


FreeCam::FreeCam(Object* owner)
	: Component(owner)
{
}


FreeCam::~FreeCam()
{
}

void FreeCam::Start()
{
}

void FreeCam::Update(float deltaTime, float totalTime)
{
	// Use left stick of joystick 0 to move camera
	float horizontal = SInput->GetAxis("Horizontal");
	float vertical = SInput->GetAxis("Vertical");

	DirectX::XMVECTOR position = object->transform->GetLocalTranslation();
	position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(object->transform->Right(), deltaTime * horizontal * 4));
	position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(object->transform->Forward(), deltaTime * vertical * 4));
	object->transform->SetLocalTranslation(position);

	// Use left stick of joystick 0 to rotate camera
	if (SInput->GetButton("RightButton"))
	{
		float rightHorizontal = SInput->GetAxis("CameraHorizontal");
		float rightVertical = SInput->GetAxis("CameraVertical");

		DirectX::XMVECTOR rotation = object->transform->GetLocalRotation();
		DirectX::XMVECTOR rotationRightAxis = DirectX::XMQuaternionRotationAxis(object->transform->Right(), -deltaTime * rightVertical);
		DirectX::XMVECTOR rotationUpAxis = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), deltaTime * rightHorizontal);
		rotation = DirectX::XMQuaternionMultiply(rotation, rotationRightAxis);
		rotation = DirectX::XMQuaternionMultiply(rotation, rotationUpAxis);
		object->transform->SetLocalRotation(rotation);
	}
}
