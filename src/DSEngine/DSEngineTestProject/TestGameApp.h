/**
 * @file TestGameApp.h
 * @author Victor Shu
 * @brief This file contains the example/test class for 
 * the game that is using DSEngine.
 * 
 * @version 0.1
 * @date 2019/02/20
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include "DSEngineApp.h"

/**
 * @class TestGameApp
 * 
 * @brief Derived from DSEngineApp
 * 
 * @todo This class should contain actual game logic
 * 
 */
class TestGameApp :
	public DSEngineApp
{
public:
	/**
	 * @brief Override the base class Init function
	 * Should initialize game-specific things
	 *
	 * @param hInstance The instance of the app
	 * @param lpCmdLine The command line parameters of the app
	 * @param hWnd The window handle
	 * @param screenWidth Screen width
	 * @param screenHeight Screen height
	 * @return true if initialization succeeded, or false
	 */
	bool Init(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenHeight) override;
};

