#include <iostream>
#include "TestGameApp.h"

bool TestGameApp::Init(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenHeight)
{
	DSEngineApp::Init(hInstance, lpCmdLine, hWnd, screenWidth, screenHeight);
	// Stub logic
	std::cout << "TestGameApp Init" << std::endl;
	return true;
}
