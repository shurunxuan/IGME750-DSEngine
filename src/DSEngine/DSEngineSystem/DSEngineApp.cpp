#include <iostream>
#include "DSEngineApp.h"

DSEngineApp* App = nullptr;

DSEngineApp::DSEngineApp()
{
	// Since the game should initialize its own derived
	// instance before the program entry point, this
	// is called before the DSEngine function.
	// Thus, it is safe to use App pointer inside DSEngine
	// function.
	App = this;

	InitLogger();
}


DSEngineApp::~DSEngineApp()
{
	playbackThread.interrupt();
	playbackThread.join();

	StopLogger();
}

bool DSEngineApp::Init(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenHeight)
{
	audioSystem.Init();
	renderingSystem.Init(hWnd, screenWidth, screenHeight);
	inputSystem.Init();
	LOG_TRACE << "DSEngineApp Init";

	// Test play audio file
	audioSystem.PlayAudioFileNonBlock("test3.flac", playbackThread);

	return true;
}

void DSEngineApp::Loop()
{
	// Update frame time first
	// This is managed by the rendering system
	renderingSystem.UpdateTimer();
	renderingSystem.UpdateTitleBarStats();

	const float deltaTime = renderingSystem.GetDeltaTime();
	const float totalTime = renderingSystem.GetTotalTime();

	inputSystem.Update();
	renderingSystem.Update(deltaTime, totalTime);
	std::cout << "DSEngineApp::Loop()" << std::endl;
}

DSSRendering* DSEngineApp::GetRenderingSystem()
{
	return &renderingSystem;
}
