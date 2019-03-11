#include "DSFLogging.h"
#include "DSFRawInput.h"
#include "DSSAudio.h"

#include "PressSpaceToPlayAudio.h"
#include "DSFXInput.h"

#include "Scene.hpp"


PressSpaceToPlayAudio::PressSpaceToPlayAudio(Object* owner) : Component(owner)
{
}


PressSpaceToPlayAudio::~PressSpaceToPlayAudio()
{
	playbackThread.interrupt();
	playbackThread.join();
}

void PressSpaceToPlayAudio::Start()
{
	isPlaying = false;

	LOG_TRACE << "Component PressSpaceToPlayAudio added to object " << object->name << ".";
}

void PressSpaceToPlayAudio::Update(float deltaTime, float totalTime)
{
	// Only for test. Don't use FRawInput or FXInput directly after the input system is completed.
	if (FRawInput->GetKeyDown(VK_SPACE) && !isPlaying)
	{
		// Test play audio file
		SAudio->PlayAudioFileNonBlock("test3.flac", playbackThread);
		isPlaying = true;
	}

	// Use arrow keys to move object
	if (FRawInput->GetKey(VK_UP))
	{
		DirectX::XMVECTOR position = object->transform->GetTranslation();
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(0.0f, deltaTime, 0.0f, 0.0f));
		object->transform->SetTranslation(position);
	}

	if (FRawInput->GetKey(VK_DOWN))
	{
		DirectX::XMVECTOR position = object->transform->GetTranslation();
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(0.0f, -deltaTime, 0.0f, 0.0f));
		object->transform->SetTranslation(position);
	}

	if (FRawInput->GetKey(VK_LEFT))
	{
		DirectX::XMVECTOR position = object->transform->GetTranslation();
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(-deltaTime, 0.0f, 0.0f, 0.0f));
		object->transform->SetTranslation(position);
	}

	if (FRawInput->GetKey(VK_RIGHT))
	{
		DirectX::XMVECTOR position = object->transform->GetTranslation();
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(deltaTime, 0.0f, 0.0f, 0.0f));
		object->transform->SetTranslation(position);
	}

	// Use left stick of joystick 0 to move camera
	const float horizontal = FXInput->GetAxis(LX, 0);
	const float vertical = FXInput->GetAxis(LY, 0);

	DirectX::XMVECTOR position = object->GetScene()->mainCamera->transform->GetTranslation();
	position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(object->GetScene()->mainCamera->transform->Right(), deltaTime * horizontal * 2));
	position = DirectX::XMVectorAdd(position, DirectX::XMVectorScale(object->GetScene()->mainCamera->transform->Forward(), deltaTime * vertical * 2));
	object->GetScene()->mainCamera->transform->SetTranslation(position);

	// Use left stick of joystick 0 to rotate camera
	const float rightHorizontal = FXInput->GetAxis(RX, 0);
	const float rightVertical = FXInput->GetAxis(RY, 0);

	DirectX::XMVECTOR rotation = object->GetScene()->mainCamera->transform->GetRotation();
	DirectX::XMVECTOR rotationRightAxis = DirectX::XMQuaternionRotationAxis(object->GetScene()->mainCamera->transform->Right(), -deltaTime * rightVertical);
	DirectX::XMVECTOR rotationUpAxis = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), deltaTime * rightHorizontal);
	rotation = DirectX::XMQuaternionMultiply(rotation, rotationRightAxis);
	rotation = DirectX::XMQuaternionMultiply(rotation, rotationUpAxis);
	object->GetScene()->mainCamera->transform->SetRotation(rotation);

}
