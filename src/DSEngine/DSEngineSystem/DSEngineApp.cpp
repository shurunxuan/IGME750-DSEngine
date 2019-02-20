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
	return true;
}
