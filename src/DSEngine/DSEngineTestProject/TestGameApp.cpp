#include <iostream>
#include "TestGameApp.h"

TestGameApp::~TestGameApp()
{
	playbackThread.interrupt();
	playbackThread.join();
}

void TestGameApp::Init()
{
	// Stub logic
	LOG_TRACE << "TestGameApp Init";

	isPlaying = false;

	Object* newObj = CurrentActiveScene()->AddObject("NewObject");
	Transform* transformByGet = newObj->GetComponent<Transform>();
	Transform* transformDirect = newObj->transform;

}

void TestGameApp::Update(float deltaTime, float totalTime)
{
	// Only for test. Don't do this after the input system is completed.
	if (FRawInput->GetKeyDown(VK_SPACE) && !isPlaying)
	{
		// Test play audio file
		SAudio->PlayAudioFileNonBlock("test3.flac", playbackThread);
		isPlaying = true;
	}
}
