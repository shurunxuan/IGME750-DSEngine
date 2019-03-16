/**
 * @file DSSRendering.h
 * @author Victor Shu
 * @brief This file declares the class for DSEngine Rendering System.
 * @version 0.1
 * @date 2019/02/25
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
#include "DSFDirect3D.h"

/**
 * @brief The Rendering System of the DS Engine
 */
class DSENGINESYSTEM_API DSSRendering
{
public:
	/**
	 * @brief Construct a new DSSRendering object
	 * 
	 * Should initialize all pointers as nullptr
	 */
	DSSRendering();
	/**
	 * @brief Destroy the DSSRendering object
	 * 
	 * Should delete or release or free all pointers
	 */
	~DSSRendering();

		/**
	 * @brief Copy constructor of DSSRendering is deleted
	 * since the class is meant to be a singleton
	 * 
	 * @param v Another instance.
	 */
	DSSRendering(const DSSRendering& v) = delete;

	/**
	 * @brief Move constructor of DSSRendering is deleted
	 * since the class is meant to be a singleton
	 * 
	 * @param v Another instance
	 */
	DSSRendering(DSSRendering&& v) = delete;

	/**
	 * @brief Copy assignment operator of DSSRendering is deleted
	 * since the class is meant to be a singleton
	 * 
	 * @param v Another instance
	 */
	DSSRendering& operator=(const DSSRendering& v) = delete;

	/**
	 * @brief Move assignment operator of DSSRendering is deleted
	 * since the class is meant to be a singleton
	 * 
	 * @param v Another instance
	 */
	DSSRendering& operator=(DSSRendering&& v) = delete;

	/**
	 * @brief Actual initialization of the rendering system
	 * 
	 * @param hWnd The window handle
	 * @param screenWidth Screen width
	 * @param screenHeight Screen height
	 * @return S_OK if initialization succeeded, or other
	 */
	HRESULT Init(HWND hWnd, unsigned int screenWidth, unsigned int screenHeight);

	/**
	 * @brief Resize the rendering system
	 *
	 * @param screenWidth Screen width
	 * @param screenHeight Screen height
	 * @return S_OK if resize succeeded, or other
	 */
	HRESULT OnResize(unsigned int screenWidth, unsigned int screenHeight);

	/**
	 * @brief Update performance timer
	 */
	void UpdateTimer();

	/**
	 * @brief Puts debug info in the title bar
	 */
	void UpdateTitleBarStats();

	/**
	 * @brief Get the time cost by one frame
	 *
	 * @return Time cost by one frame
	 */
	float GetDeltaTime() const;

	/**
	 * @brief Get the time since the start of the game
	 *
	 * @return Time since the start of the game
	 */
	float GetTotalTime() const;

	/**
	 * @brief Update the rendering system every frame
	 * 
	 * @param deltaTime The time cost by one frame
	 * @param totalTime The time since the game started
	 * 
	 * @todo This is currently a place holder that reroute the call to DSFDirect3D::Draw
	 */
	void Update(float deltaTime, float totalTime);

private:
	/**
	 * @brief The Direct3D Framework reference 
	 */
	DSFDirect3D direct3D;

	/**
	 * @brief Performance counter
	 */
	double perfCounterSeconds;
	/**
	 * @brief Total time from the start of the game
	 */
	float totalTime;
	/**
	 * @brief The time of one frame
	 */
	float deltaTime;
	/**
	 * @brief Timestamp of the start of the game
	 */
	__int64 startTime;
	/**
	 * @brief Current timestamp
	 */
	__int64 currentTime;
	/**
	 * @brief Previous timestamp
	 */
	__int64 previousTime;

	/**
	 * @brief FPS frame counter
	 */
	int fpsFrameCount;
	/**
	 * @brief Time elapsed since last frame
	 */
	float fpsTimeElapsed;

	SimpleVertexShader* shadowVertexShader;
};

/**
 * @brief The pointer that points to the singleton
 * of the Rendering System
 */
extern DSENGINESYSTEM_API DSSRendering* SRendering;