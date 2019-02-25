/**
 * @file DSEngine.h
 * @author Victor Shu
 * @brief This file contains the forwarded entry point
 * from the game app.
 * @version 0.1
 * @date 2019/02/20
 *
 * @copyright Copyright (c) 2019
 *
 */
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#ifdef DSENGINESYSTEM_EXPORTS
#define DSENGINESYSTEM_API __declspec(dllexport)
#else
#define DSENGINESYSTEM_API __declspec(dllimport)
#endif


extern "C" 
{
	/**
	 * @brief Forwarded entry point from the game app
	 * Initialize the Win32 stuff. Actual game loop happens here
	 *
	 * @param hInstance Handle to the current instance of the application
	 * @param hPrevInstance Handle to the previous instance of the application. For a Win32-based application, this parameter is always NULL
	 * @param lpCmdLine Pointer to a null-terminated string specifying the command line for the application, excluding the program name. To retrieve the entire command line, use the GetCommandLine() function
	 * @param nCmdShow Specifies how the window is to be shown
	 * @return If the function succeeds, terminating when it receives a WM_QUIT message, it should return the exit value contained in that message's wParam parameter. If the function terminates before entering the message loop, it should return zero
	 */
	extern INT DSENGINESYSTEM_API WINAPI DSEngine(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
}