#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

#include "DSEngine.h"
#include "RacingGameApp.h"

RacingGameApp GameApp;

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine,
	int nCmdShow)
{
	// Forward the entry point to the engine.
	return DSEngine(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
