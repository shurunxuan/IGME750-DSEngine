/**
 * @file DSFRawInput.h
 * @author Victor Shu
 * @brief This file declares the framework class for Raw Input.
 * @version 0.1
 * @date 2019/03/02
 *
 * @copyright Copyright (c) 2019
 *
 */

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#ifdef DSENGINEFRAMEWORK_EXPORTS
#define DSENGINEFRAMEWORK_API __declspec(dllexport)
#else
#define DSENGINEFRAMEWORK_API __declspec(dllimport)
#endif

/**
 * @brief The Raw Input Framework of the DS Engine
 * 
 */
class DSENGINEFRAMEWORK_API DSFRawInput
{
public:
	/**
	 * @brief Construct a new DSFRawInput object
     * 
     * Should initialize all pointers as nullptr
	 */
	DSFRawInput();
	/**
	 * @brief Destroy the DSFRawInput object
	 * 
     * Should delete or release or free all pointers
     */
	~DSFRawInput();

    /**
     * @brief Copy constructor of DSFRawInput is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSFRawInput(const DSFRawInput& v) = delete;

    /**
     * @brief Move constructor of DSFRawInput is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSFRawInput(DSFRawInput&& v) = delete;

    /**
     * @brief Copy assignment operator of DSFRawInput is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSFRawInput& operator=(const DSFRawInput& v) = delete;

    /**
     * @brief Move assignment operator of DSFRawInput is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSFRawInput& operator=(DSFRawInput&& v) = delete;

	/**
	 * @brief The actual initialization of Raw Input Framework 
	 * 
	 */
	void Init();
	/**
	 * @brief Update the state of the keyboard and mouse
	 * 
	 */
	void Update();

	/**
	 * @brief The callback called when there is a keyboard input
	 * 
	 * @param raw The structure that contains keyboard input data
	 */
	void OnKeyboardInput(RAWKEYBOARD raw);
	/**
	 * @brief The callback called when there is a mouse input
	 * 
	 * @param raw The structure that contains mouse input data
	 */
	void OnMouseInput(RAWMOUSE raw);

private:
	/**
	 * @brief Stored keyboard input data
	 * 
	 */
	RAWKEYBOARD keyboardInput;
	/**
	 * @brief Stored mouse input data
	 * 
	 */
	RAWMOUSE mouseInput;
};

/**
 * @brief The pointer that points to the singleton
 * of the Raw Input Framework
 */
extern DSENGINEFRAMEWORK_API DSFRawInput* FRawInput;