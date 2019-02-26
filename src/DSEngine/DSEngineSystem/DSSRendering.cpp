#include "DSSRendering.h"
#include "DSFLogging.h"
#include <iostream>


DSSRendering::DSSRendering()
{
}


DSSRendering::~DSSRendering()
{
}

HRESULT DSSRendering::Init(HWND hWnd, unsigned int screenWidth, unsigned int screenHeight)
{
	HRESULT hr = direct3D.Init(hWnd, screenWidth, screenHeight);
	LOG_TRACE << "DSEngine Rendering System Initialized!";
	return hr;
}

HRESULT DSSRendering::OnResize(unsigned int screenWidth, unsigned int screenHeight)
{
	return direct3D.OnResize(screenWidth, screenHeight);
}

void DSSRendering::Update()
{
	direct3D.Draw();
}
