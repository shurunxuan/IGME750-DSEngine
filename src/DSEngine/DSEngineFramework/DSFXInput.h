/**
 * @file DSFXInput.h
 * @author Victor Shu
 * @brief This file declares the framework class for XInput.
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
#include <xinput.h>
#include <string>

#ifdef DSENGINEFRAMEWORK_EXPORTS
#define DSENGINEFRAMEWORK_API __declspec(dllexport)
#else
#define DSENGINEFRAMEWORK_API __declspec(dllimport)
#endif

/**
 * @brief The button codes that maps to physical buttons on a gamepad
 * 
 */
enum DSJoystickButtonCode
{
    DPAD_UP = 0x0001,			// DPad Up
    DPAD_DOWN = 0x0002,			// DPad Down
    DPAD_LEFT = 0x0004, 		// DPad Left
    DPAD_RIGHT = 0x0008,		// DPad Right
    START = 0x0010,				// Start / Menu
    BACK = 0x0020,				// Back / View
    LEFT_THUMB = 0x0040,		// Left Thumb Button
    RIGHT_THUMB = 0x0080,		// Right Thumb Button
    LEFT_SHOULDER = 0x0100,		// Left Shoulder Button / LB
    RIGHT_SHOULDER = 0x0200,	// Right Shoulder Button / RB
    RESERVED_0 = 0x0400,		// Reserved
    RESERVED_1 = 0x0800,		// Reserved
    A = 0x1000,					// A Button
    B = 0x2000,					// B Button
    X = 0x4000,					// X Button
    Y = 0x8000					// Y Button
};

/**
 * @brief The codes of analog axis that maps to a gamepad
 * 
 */
enum DSJoystickAxisCode
{
    LT,				// Left Trigger
    RT,				// Right Trigger
    LX,				// Left Thumb Horizontal
    LY,				// Left Thumb Vertical
    RX,				// Right Thumb Horizontal
    RY,				// Right Thumb Vertical
    ERROR_AXIS		// Doesn't exist
};

/**
 * @brief The XInput Framework of the DS Engine
 * 
 */
class DSENGINEFRAMEWORK_API DSFXInput
{
public:
    /**
     * @brief Construct a new DSFXInput object
     * 
     * Should initialize all pointers as nullptr
     */
    DSFXInput();
        /**
     * @brief Destroy the DSFXInput object
     * 
     * Should delete or release or free all pointers
     */
    ~DSFXInput();

    /**
     * @brief Copy constructor of DSFXInput is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSFXInput(const DSFXInput& v) = delete;

    /**
     * @brief Move constructor of DSFXInput is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSFXInput(DSFXInput&& v) = delete;

    /**
     * @brief Copy assignment operator of DSFXInput is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSFXInput& operator=(const DSFXInput& v) = delete;

    /**
     * @brief Move assignment operator of DSFXInput is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSFXInput& operator=(DSFXInput&& v) = delete;

    /**
     * @brief The actual initialization of XInput Framework 
     * 
     */
    void Init();
    /**
     * @brief Update the state of the gamepads
     * 
     */
    void Update();

    /**
     * @brief Called when a WM_DEVICECHANGE is presented.
     * 
     * Re-poll the connected controllers
     */
    void OnDeviceChange();

    /**
     * @brief Get the state of if a button is being held
     * 
     * @param buttonCode Button code
     * @param player Joystick number, -1 for all joysticks
     * @return The state of a button is being held
     */
    bool GetButton(DSJoystickButtonCode buttonCode, int player) const;
    /**
     * @brief Get the state of if a button is pressed
     * 
     * @param buttonCode Button code
     * @param player Joystick number, -1 for all joysticks
     * @return The state of a button is pressed
     */
    bool GetButtonDown(DSJoystickButtonCode buttonCode, int player) const;
    /**
     * @brief Get the state of if a button is released
     * 
     * @param buttonCode Button code
     * @param player Joystick number, -1 for all joysticks
     * @return The state of a button is released
     */
    bool GetButtonUp(DSJoystickButtonCode buttonCode, int player) const;
    /**
     * @brief Get all the button pressed events
     * 
     * @param player Joystick number, -1 for the first joysitck
     * @return Packed data of if a button is pressed 
     */
    WORD GetDownEvent(int player) const;
    /**
     * @brief Get all the button released events
     * 
     * @param player Joystick number, -1 for the first joysitck
     * @return Packed data of if a button is released 
     */
    WORD GetUpEvent(int player) const;
    /**
     * @brief Get the processed axis data
     * 
     * @param axisCode Axis code
     * @param player Joystick number, -1 for the first joysitck
     * @param deadZone Dead zone, default to -1 (XInput default value will be used)
     * @return float The axis data, ranging from 0.0f to 1.0f. 0.0f if the raw data falls in the dead zone
     */
    float GetAxis(DSJoystickAxisCode axisCode, int player, float deadZone = -1) const;
    /**
     * @brief Get the raw axis data
     * 
     * @param axisCode Axis code
     * @param player Joystick number, -1 for the first joysitck
     * @return float The axis data, ranging from 0.0f to 1.0f
     */
    float GetRawAxis(DSJoystickAxisCode axisCode, int player) const;

    /**
     * @brief Convert the integer button code to enum value
     * 
     * @param code Integer button code
     * @return DSJoystickButtonCode Enum button code
     */
    static DSJoystickButtonCode GetButtonCode(unsigned int code);
    /**
     * @brief Convert the integer axis code to enum value
     * 
     * @param code Integer axis code
     * @return DSJoystickAxisCode Enum axis code
     */
    static DSJoystickAxisCode GetAxisCode(unsigned int code);
    /**
     * @brief Get the name of a button from its button code
     * 
     * @param buttonCode The button code
     * @return std::string The name of the button
     */
    static std::string GetButtonName(DSJoystickButtonCode buttonCode);
    /**
     * @brief Get the name of a axis from its axis code
     * 
     * @param axisCode The axis code
     * @return std::string The name of the axis
     */
    static std::string GetAxisName(DSJoystickAxisCode axisCode);

private:
    /**
     * @brief Current state of the buttons held
     * 
     */
    XINPUT_STATE buttonState[XUSER_MAX_COUNT]{};
    /**
     * @brief Current state of the buttons pressed
     * 
     */
    XINPUT_STATE buttonDownState[XUSER_MAX_COUNT]{};
    /**
     * @brief Current state of the buttons released
     * 
     */
    XINPUT_STATE buttonUpState[XUSER_MAX_COUNT]{};

    /**
     * @brief Last state of the buttons held
     * 
     */
    XINPUT_STATE lastState[XUSER_MAX_COUNT]{};
    /**
     * @brief Current connected joysticks
     * 
     */
    bool connected[XUSER_MAX_COUNT]{};
};

/**
 * @brief The pointer that points to the singleton
 * of the XInput Framework
 */
extern DSENGINEFRAMEWORK_API DSFXInput* FXInput;