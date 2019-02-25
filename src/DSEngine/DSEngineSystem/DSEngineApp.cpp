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
}


DSEngineApp::~DSEngineApp()
{
}

bool DSEngineApp::Init(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenHeight)
{
	renderingSystem.Init(hWnd, screenWidth, screenHeight);
	OutputDebugString(TEXT("DSEngineApp Init\n"));
	return true;
}

void DSEngineApp::Loop()
{
	std::cout << "DSEngineApp::Loop()" << std::endl;
}
