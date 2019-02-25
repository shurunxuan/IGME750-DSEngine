#include "DSSRendering.h"
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
	OutputDebugString(TEXT("DSEngine Rendering System Initialized!\n"));
	return hr;
}
