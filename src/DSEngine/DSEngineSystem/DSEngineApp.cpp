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
	StopLogger();
}

bool DSEngineApp::Initialize(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenHeight)
{
	width = screenWidth;
	height = screenHeight;

	audioSystem.Init();
	renderingSystem.Init(hWnd, screenWidth, screenHeight);
	inputSystem.Init(hWnd);
	LOG_TRACE << "DSEngineApp Init";

	Init();

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

	// This contains the actual game logic
	currentScene.Update(deltaTime, totalTime);

	renderingSystem.Update(deltaTime, totalTime);
}

Scene* DSEngineApp::CurrentActiveScene()
{
	return &currentScene;
}