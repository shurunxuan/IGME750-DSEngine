#include "DSFLogging.h"
#include "DSFRawInput.h"
#include "DSSAudio.h"

#include "PressSpaceToPlayAudio.h"
#include "DSFXInput.h"


PressSpaceToPlayAudio::PressSpaceToPlayAudio(Object* owner): Component(owner)
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
	// Only for test. Don't do this after the input system is completed.
	if (FRawInput->GetKeyDown(VK_SPACE) && !isPlaying)
	{
		// Test play audio file
		SAudio->PlayAudioFileNonBlock("test3.flac", playbackThread);
		isPlaying = true;
	}

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

	const float horizontal = FXInput->GetAxis(LX, 0);
	const float vertical = FXInput->GetAxis(LY, 0);
	if (horizontal != 0 || vertical != 0)
	{
		DirectX::XMVECTOR position = object->transform->GetTranslation();
		position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(deltaTime * horizontal, deltaTime * vertical, 0.0f, 0.0f));
		object->transform->SetTranslation(position);
	}
}
