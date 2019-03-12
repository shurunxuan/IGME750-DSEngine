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
}

void PressSpaceToPlayAudio::Update(float deltaTime, float totalTime)
{
	// Only for test. Don't use FRawInput or FXInput directly after the input system is completed.
	if (FRawInput->GetKeyDown(VK_SPACE) && !isPlaying)
	{
		// Test play audio file
		SAudio->PlayAudioFileNonBlock("Assets/test3.flac", playbackThread);
		isPlaying = true;
	}


}
