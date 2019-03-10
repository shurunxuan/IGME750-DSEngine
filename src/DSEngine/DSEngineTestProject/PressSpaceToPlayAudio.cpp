#include "DSFLogging.h"
#include "DSFRawInput.h"
#include "DSSAudio.h"

#include "PressSpaceToPlayAudio.h"


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
}
