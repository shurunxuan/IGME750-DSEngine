#include "DSSRendering.h"
#include "DSFLogging.h"
#include <iostream>


DSSRendering::DSSRendering()
{
}


DSSRendering::~DSSRendering()
{
}

HRESULT DSSRendering::Init(HWND hWnd, unsigned screenWidth, unsigned screenHeight)
{
	HRESULT hr = direct3D.Init(hWnd, screenWidth, screenHeight);
	LOG_TRACE << "DSEngine Rendering System Initialized!";
	return hr;
}
