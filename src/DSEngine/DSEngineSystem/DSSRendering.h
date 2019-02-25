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

private:
	/**
	 * @brief Direct3D Framework instance 
	 */
	DSFDirect3D direct3D;
};

