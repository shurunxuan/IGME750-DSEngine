/**
 * @file DSEngineApp.h
 * @author Victor Shu
 * @brief This file contains the class that
 * the game app should derive from.
 * @version 0.1
 * @date 2019/02/20
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once

#ifdef DSENGINESYSTEM_EXPORTS
#define DSENGINESYSTEM_API __declspec(dllexport)
#else
#define DSENGINESYSTEM_API __declspec(dllimport)
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include "DSSRendering.h"
#include "DSSAudio.h"
#include "DSSInput.h"
#include "DSSPhysics.h"

#include "Scene.hpp"
#include "SimpleShader.hpp"

/**
 * @brief The app class. Game should derive from this
 */
class DSENGINESYSTEM_API DSEngineApp
{
public:
	/**
	 * @brief Construct a new DSEngineApp object
	 * 
	 * Should initialize all pointers as nullptr
	 */
	DSEngineApp();
	/**
	 * @brief Destroy the DSEngineApp object
	 * 
	 * Should delete or release or free all pointers
	 */
	virtual ~DSEngineApp();

	/**
	 * @brief Copy constructor of DSEngineApp is deleted
	 * since the class is meant to be a singleton
	 * 
	 * @param v Another instance.
	 */
	DSEngineApp(const DSEngineApp& v) = delete;

	/**
	 * @brief Move constructor of DSEngineApp is deleted
	 * since the class is meant to be a singleton
	 * 
	 * @param v Another instance
	 */
	DSEngineApp(DSEngineApp&& v) = delete;

	/**
	 * @brief Copy assignment operator of DSEngineApp is deleted
	 * since the class is meant to be a singleton
	 * 
	 * @param v Another instance
	 */
	DSEngineApp& operator=(const DSEngineApp& v) = delete;

	/**
	 * @brief Move assignment operator of DSEngineApp is deleted
	 * since the class is meant to be a singleton
	 * 
	 * @param v Another instance
	 */
	DSEngineApp& operator=(DSEngineApp&& v) = delete;

	/**
	 * @brief Actual initialization of the app
	 * 
	 * @param hInstance The instance of the app
	 * @param lpCmdLine The command line parameters of the app
	 * @param hWnd The window handle
	 * @param screenWidth Screen width
	 * @param screenHeight Screen height
	 * @return true if initialization succeeded, or false
	 */
	virtual bool Initialize(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenHeight);

	/**
	 * @brief Initialize of the game logic
	 * 
	 */
	virtual void Init() = 0;

	/**
	 * @brief The game loop. Called in function DSEngine
	 * 
	 * @todo Add/change return type or parameters as needed
	 */
	void Loop();

	/**
	 * @brief Get the current active scene
	 * 
	 * @todo This only returns App::currentScene currently. If we are going to have multiple scenes, there should be a scene manager.
	 */
	Scene* CurrentActiveScene();

private:
	/**
	 * @brief The Rendering System reference
	 */
	DSSRendering renderingSystem;

	/**
	 * @brief The Audio System reference
	 */
	DSSAudio audioSystem;

	/**
	 * @brief The Input System reference
	 */
	DSSInput inputSystem;

	/**
	 * @brief Current active scene
	 */
	Scene currentScene;

	/**
	 * @brief The Physics System reference
	 */
	DSSPhysics physicsSystem;

	/**
	 * @brief The vertex shader of the default material
	 * 
	 */
	SimpleVertexShader* vertexShader;

	/**
	 * @brief The pixel shader of the default material
	 * 
	 */
	SimplePixelShader* pbrPixelShader;

protected:
	/**
	 * @brief Screen width
	 * 
	 */
	int width;
	
	/**
	 * @brief Screen height
	 * 
	 */
	int height;

	/**
	 * @brief Direct3D 11 device
	 * 
	 */
	ID3D11Device* device;

	/**
	 * @brief Direct3D 11 device context
	 * 
	 */
	ID3D11DeviceContext* context;
};

/**
 * @brief The pointer that points to the singleton
 * of the current game app
 */
extern DSENGINESYSTEM_API DSEngineApp* App;

