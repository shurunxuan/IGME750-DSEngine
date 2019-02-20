#pragma once
#include "DSEngineApp.h"

class TestGameApp :
	public DSEngineApp
{
public:
	bool Init(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenHeight) override;
};

