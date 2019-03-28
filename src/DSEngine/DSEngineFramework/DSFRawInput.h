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
#include <string>

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
     * @param hWnd The handle to the window
     */
    void Init(HWND hWnd);
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

    /**
     * @brief Get the state of the key being held
     * 
     * @param VKey Virtual Key Code
     * @return true If the key is being held
     */
    bool GetKey(int VKey);
    /**
     * @brief Get the state of the key being pressed
     * 
     * @param VKey Virtual Key Code
     * @return true If the key is being pressed
     */
    bool GetKeyDown(int VKey);
    /**
     * @brief Get the state of the key being released
     * 
     * @param VKey Virtual Key Code
     * @return true If the key is being released
     */
    bool GetKeyUp(int VKey);

    /**
     * @brief Get the state of the Mouse Button being held
     * 
     * @param button The mouse button number (0 for left, 1 for right, 2 for middle)
     * @return true If the button is being held
     */
    bool GetMouseButton(int button);
    /**
     * @brief Get the state of the Mouse Button being pressed
     * 
     * @param button The mouse button number (0 for left, 1 for right, 2 for middle)
     * @return true If the button is being pressed
     */
    bool GetMouseButtonDown(int button);
    /**
     * @brief Get the state of the Mouse Button being released
     * 
     * @param button The mouse button number (0 for left, 1 for right, 2 for middle)
     * @return true If the button is being released
     */
    bool GetMouseButtonUp(int button);

    /**
     * @brief Get the x position of the mouse pointer
     * 
     * Don't use this if the mouse pointer is hidden. Use GetMouseDeltaX instead.
     * 
     * @return LONG The x position of the mouse pointer
     */
    LONG GetMousePosX() const;
    /**
     * @brief Get the y position of the mouse pointer
     * 
     * Don't use this if the mouse pointer is hidden. Use GetMouseDeltaY instead.
     * 
     * @return LONG The y position of the mouse pointer
     */
    LONG GetMousePosY() const;
    /**
     * @brief Get the x delta of the mouse pointer
     * 
     * @return LONG The x delta of the mouse pointer
     */
    LONG GetMouseDeltaX() const;
    /**
     * @brief Get the y delta of the mouse pointer
     * 
     * @return LONG The y delta of the mouse pointer
     */
    LONG GetMouseDeltaY() const;
    /**
     * @brief Get the wheel delta of the mouse
     * 
     * @return USHORT The wheel delta of the mouse
     */
    USHORT GetMouseWheelDelta() const;

    /**
     * @brief Get the name of a Virtual Key Code
     * 
     * @param VKey Virtual Key Code
     * @return std::string Name of the Virtual Key Code
     */
    static std::string GetKeyName(int VKey);

private:
    /**
     * @brief Handle to the window
     * 
     */
    HWND hWnd;

    /**
     * @brief Current state of the key being held
     * 
     */
    bool keyStates[256]{};
    /**
     * @brief Current state of the key being pressed
     * 
     */
    bool keyDownStates[256]{};
    /**
     * @brief Current state of the key being released
     * 
     */
    bool keyUpStates[256]{};

    /**
     * @brief Last state of the key being held
     * 
     */
    bool lastKeyStates[256]{};

    /**
     * @brief X delta of the mouse pointer
     * 
     */
    LONG mouseDeltaX;
    /**
     * @brief Y delta of the mouse pointer
     * 
     */
    LONG mouseDeltaY;
    /**
     * @brief Wheel delta of the mouse
     * 
     */
    USHORT mouseDeltaWheel;

    /**
     * @brief Record if mouse moved during the update
     */
    bool mouseMoved;

    /**
     * @brief Current state of the mouse button being held
     * 
     */
    bool mouseButtonStates[5]{};
    /**
     * @brief Current state of the mouse button being pressed
     * 
     */
    bool mouseButtonDownStates[5]{};
    /**
     * @brief Current state of the mouse button being released
     * 
     */
    bool mouseButtonUpStates[5]{};

    /**
     * @brief Last state of the mouse button being held
     * 
     */
    bool lastMouseButtonStates[5]{};
};

/**
 * @brief The pointer that points to the singleton
 * of the Raw Input Framework
 */
extern DSENGINEFRAMEWORK_API DSFRawInput* FRawInput;