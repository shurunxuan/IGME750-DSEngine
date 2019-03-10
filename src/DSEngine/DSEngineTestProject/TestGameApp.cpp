#include <iostream>
#include "TestGameApp.h"
#include "PressSpaceToPlayAudio.h"

TestGameApp::~TestGameApp()
{
}

void TestGameApp::Init()
{
	// Stub logic
	LOG_TRACE << "TestGameApp Init";

	Object* newObj = CurrentActiveScene()->AddObject("NewObject");
	PressSpaceToPlayAudio* newComponent = newObj->AddComponent<PressSpaceToPlayAudio>();
}
