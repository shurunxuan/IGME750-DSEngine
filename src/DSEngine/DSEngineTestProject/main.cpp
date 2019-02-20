#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <iostream>
#include <Windows.h>
#include "DSEngine.h"
#include "DSSTestSystem.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine,
	int nCmdShow)
{
	return DSEngine(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
