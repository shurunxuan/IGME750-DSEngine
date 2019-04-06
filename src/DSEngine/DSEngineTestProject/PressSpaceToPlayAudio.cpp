#include "DSSAudio.h"

#include "PressSpaceToPlayAudio.h"

#include "Scene.hpp"
#include "DSSInput.h"


PressSpaceToPlayAudio::PressSpaceToPlayAudio(Object* owner) : Component(owner)
{
}


PressSpaceToPlayAudio::~PressSpaceToPlayAudio()
{

}

void PressSpaceToPlayAudio::Start()
{

}

void PressSpaceToPlayAudio::Update(float deltaTime, float totalTime)
{
	// Only for test. Don't use FRawInput or FXInput directly after the input system is completed.
	//if (SInput->GetButtonDown("PlayAudio") && !isPlaying)
	//{
	//	// Test play audio file
	//	SAudio->PlayAudioFileNonBlock("Assets/test3.flac", playbackThread);
	//	isPlaying = true;
	//}

	if (SInput->GetButtonDown("PlayAudio1"))
	{
		source1->Play();
	}

	if (SInput->GetButtonDown("PauseAudio1"))
	{
		source1->Pause();
	}

	if (SInput->GetButtonDown("StopAudio1"))
	{
		source1->Stop();
	}

	if (SInput->GetButtonDown("PlayAudio2"))
	{
		source2->Play();
	}

	if (SInput->GetButtonDown("PauseAudio2"))
	{
		source2->Pause();
	}

	if (SInput->GetButtonDown("StopAudio2"))
	{
		source2->Stop();
	}
}
