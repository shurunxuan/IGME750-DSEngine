#include "DSEngine.h"
#include "DSEngineApp.h"

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

INT WINAPI DSEngine(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	InitLogger();
	// Enable memory leak detection as a quick and dirty
	// way of determining if we forgot to clean something up
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	// this struct holds information for the window class
	WNDCLASSEX wc;

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = TEXT("DSEngine");

	// register the window class
	RegisterClassEx(&wc);

	RECT wr = { 0, 0, 500, 400 };    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size


	// create the window and use the result as the handle
	HWND hWnd = CreateWindowEx(NULL,
		TEXT("DSEngine"), // name of the window class
		TEXT("DSEngine App"), // title of the window
		WS_OVERLAPPEDWINDOW, // window style
		300, // x-position of the window
		300, // y-position of the window
		wr.right - wr.left,    // width of the window
		wr.bottom - wr.top,    // height of the window
		nullptr, // we have no parent window, NULL
		nullptr, // we aren't using menus, NULL
		hInstance, // application handle
		nullptr);    // used with multiple windows, NULL

	// display the window on the screen
	ShowWindow(hWnd, nCmdShow);

	// TODO: Initialize DSEngineApp Here.
	App->Init(hInstance, lpCmdLine, hWnd, wr.right - wr.left, wr.bottom - wr.top);


	// enter the main loop:

	// this struct holds Windows event messages
	MSG msg;

	// Enter the infinite message loop
	while (TRUE)
	{
		// Check to see if any messages are waiting in the queue
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);

			// check to see if it's time to quit
			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			// TODO: Run game code here
			// ...
			// ...
			App->Loop();
		}
	}

	// return this part of the WM_QUIT message to Windows
	return INT(msg.wParam);
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
	case WM_SIZE:
	{
		// Don't adjust anything when minimizing,
		// since we end up with a width/height of zero
		// and that doesn't play well with DirectX
		if (wParam == SIZE_MINIMIZED)
			return 0;

		// Save the new client area dimensions.
		unsigned width = LOWORD(lParam);
		unsigned height = HIWORD(lParam);

		// If DX is initialized, resize 
		// our required buffers
		App->GetRenderingSystem()->OnResize(width, height);

		return 0;
	}
	// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	}
	default:
		break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}
