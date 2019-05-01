/**
 * @file DSSInput.h
 * @author Victor Shu
 * @brief This file declares the class for DSEngine Input System.
 * @version 0.1
 * @date 2019/03/03
 *
 * @copyright Copyright (c) 2019
 *
 */

#pragma once
#include "DSFXInput.h"
#include "DSFRawInput.h"
#include <functional>
#pragma warning(disable:4251)

#ifdef DSENGINESYSTEM_EXPORTS
#define DSENGINESYSTEM_API __declspec(dllexport)
#else
#define DSENGINESYSTEM_API __declspec(dllimport)
#endif

/**
 * @brief The types of inputs
 * 
 */
enum DSENGINESYSTEM_API InputType
{
	Button,					// Keyboard, mouse or joystick button
	Movement,				// Mouse movement
	Axis					// Joystick axes
};

/**
 * @brief The axis code if the input type is not Button
 * 
 */
enum DSENGINESYSTEM_API DSAxisCode
{
	MouseX,					// Mouse X
	MouseY,					// Mouse Y
	MouseWheel,				// Mouse Wheel
	JoystickLT,				// Left Trigger
	JoystickRT,				// Right Trigger
	JoystickLX,				// Left Thumb Horizontal
	JoystickLY,				// Left Thumb Vertical
	JoystickRX,				// Right Thumb Horizontal
	JoystickRY,				// Right Thumb Vertical
	ErrorAxis				// Doesn't exist
};

/**
 * @brief Class that represents a input
 * 
 */
class DSENGINESYSTEM_API DSSInputAxis
{
public:
	friend class DSSInput;

private:
	/**
	 * @brief Name of the input
	 * 
	 */
	std::string name;
	/**
	 * @brief Positive button of the input
	 * 
	 */
	std::string posButton;
	/**
	 * @brief Negative button of the input
	 * 
	 */
	std::string negButton;
	/**
	 * @brief Alternative positive button
	 * 
	 */
	std::string altPosButton;
	/**
	 * @brief Alternative negative button
	 * 
	 */
	std::string altNegButton;
	/**
	 * @brief How fast the input re-centers. This property applies only when the type is Button
	 * 
	 */
	float gravity;
	/**
	 * @brief Deadzone
	 * 
	 */
	float dead;
	/**
	 * @brief If the input type is Button, this represents the respond speed; if the input type is Movement, this will be multiplied to the raw movement data
	 * 
	 */
	float sensitivity;
	/**
	 * @brief Invert the input or not
	 * 
	 */
	bool invert;
	/**
	 * @brief The input type
	 * 
	 */
	InputType type;
	/**
	 * @brief The axis to be used if the input type is not Button
	 * 
	 */
	DSAxisCode axis;
	/**
	 * @brief The joystick to be used if the input requires the joystick
	 * 
	 */
	int joyNum;

	/**
	 * @brief The input system pointer
	 * 
	 */
	DSSInput* inputSystem;

	/**
	 * @brief The smoothed axis value
	 * 
	 */
	float processedAxisValue;

	/**
	 * @brief The get button functions. 1: positive, 2: altPositive, 3: negative, 4: altNegative
	 * 
	 */
	std::function<bool()> getButton[4];
	/**
	 * @brief The get button down functions. 1: positive, 2: altPositive, 3: negative, 4: altNegative
	 * 
	 */
	std::function<bool()> getButtonDown[4];
	/**
	 * @brief The get button up functions. 1: positive, 2: altPositive, 3: negative, 4: altNegative
	 * 
	 */
	std::function<bool()> getButtonUp[4];

	/**
	 * @brief The get raw axis data function
	 * 
	 */
	std::function<float()> getRawAxis;

	/**
	 * @brief Bind the functions for buttons
	 * 
	 * @param buttonName The name of the button
	 * @param buttonFuncIndex The function index to be bound. 1: positive, 2: altPositive, 3: negative, 4: altNegative
	 */
	void BindButtonCallback(const std::string& buttonName, int buttonFuncIndex);

	/**
	 * @brief Bind the functions for buttons and axes
	 * 
	 */
	void BindCallbackFunctions();

	/**
	 * @brief Update the smoothed value of the axis
	 * 
	 * @param deltaTime The time between two asynchronized input system updates
	 */
	void Update(float deltaTime);

public:
	/**
	 * @brief Construct a new DSSInputAxis object
	 * 
	 * @param name The name of the input
	 * @param posButton The positive button
	 * @param negButton The negative button
	 * @param altPosButton The alternative positive button
	 * @param altNegButton The alternative negative button
	 * @param gravity The speed the input re-centers. This property applies only when the type is Button
	 * @param dead Deadzone of an axis
	 * @param sensitivity The speed of a Button type of input responds, or the scale factor of a Movement type of input
	 * @param invert Invert the axis/button
	 * @param type Button: any kind of button (keyboard, joystick, mouse); Movement: mouse movement; Axis: axis on a joystick
	 * @param axis The hardware axis if the type is not Button
	 * @param joyNum The number of the joystick
	 * @param system The input system pointer
	 */
	DSSInputAxis(std::string name, std::string posButton, std::string negButton, std::string altPosButton, std::string altNegButton, float gravity, float dead, float sensitivity, bool invert, InputType type, DSAxisCode axis, int joyNum, DSSInput* system);
};


 /**
  * @brief The Input System of the DS Engine
  *
  */
class DSENGINESYSTEM_API DSSInput
{
public:

	friend class DSSInputAxis;

	/**
	 * @brief Construct a new DSSInput object
	 *
	 * Should set all pointers to nullptr
	 */
	DSSInput();
	/**
	 * @brief Destroy the DSSInput object
	 *
	 * Should delete or release or free all pointers
	 */
	~DSSInput();

	/**
	 * @brief Copy constructor of DSSInput is deleted
	 * since the class is meant to be a singleton
	 *
	 * @param v Another instance
	 */
	DSSInput(const DSSInput& v) = delete;

	/**
	 * @brief Move constructor of DSSInput is deleted
	 * since the class is meant to be a singleton
	 *
	 * @param v Another instance
	 */
	DSSInput(DSSInput&& v) = delete;

	/**
	 * @brief Copy assignment operator of DSSInput is deleted
	 * since the class is meant to be a singleton
	 *
	 * @param v Another instance
	 */
	DSSInput& operator=(const DSSInput& v) = delete;

	/**
	 * @brief Move assignment operator of DSSInput is deleted
	 * since the class is meant to be a singleton
	 *
	 * @param v Another instance
	 */
	DSSInput& operator=(DSSInput&& v) = delete;

	/**
	 * @brief Actual initialization of the input system
	 *
	 * @param hWnd The handle to the window
	 */
	void Init(HWND hWnd);
	/**
	 * @brief Asynchronized Update of the input system for polls
	 *
	 * @param deltaTime The time between two asynchronized input system updates
	 */
	void AsyncUpdate(float deltaTime);

	/**
	 * @brief Synchronized Update of the input system for callbacks
	 *
	 * @param deltaTime The time between two synchronized input system updates
	 */
	void SyncUpdate(float deltaTime);

	/**
	 * @brief Get if the input button is being held
	 * 
	 * @param name The name of the input
	 * @return true if the input button is being held
	 */
	bool GetButton(const std::string& name);
	/**
	 * @brief Get if the input button is pressed
	 * 
	 * @param name The name of the input
	 * @return true if the input button is pressed
	 */
	bool GetButtonDown(const std::string& name);
	/**
	 * @brief Get if the input button is released
	 * 
	 * @param name The name of the input
	 * @return true if the input button is released
	 */
	bool GetButtonUp(const std::string& name);

	/**
	 * @brief Get the input axis data
	 * 
	 * @param name The name of the input
	 * @return float The corresponding axis data
	 */
	float GetAxis(const std::string& name);
	/**
	 * @brief Get the input raw axis data
	 * 
	 * @param name The name of the input
	 * @return float The corresponding raw axis data
	 */
	float GetRawAxis(const std::string& name);

	/**
	 * @brief Get the Mouse Position
	 * 
	 * @param[out] x The x position of the mouse pointer 
	 * @param[out] y The y position of the mouse pointer
	 */
	void GetMousePosition(float* x, float* y) const;

	/**
	 * @brief Set the controller vibration
	 *
	 * @param controllerNum The number of the controller, -1 means all
	 * @param leftMotorSpeed The left motor speed (low-frequency), 0.0f ~ 1.0f
	 * @param rightMotorSpeed The right motor speed (high-frequency), 0.0f ~ 1.0f
	 */
	void SetControllerRumble(int controllerNum, float leftMotorSpeed, float rightMotorSpeed);

	/**
	 * @brief Register a new input into the input system
	 * 
	 * @param name The name of the input
	 * @param posButton The positive button
	 * @param negButton The negative button
	 * @param altPosButton The alternative positive button
	 * @param altNegButton The alternative negative button
	 * @param gravity The speed the input re-centers. This property applies only when the type is Button
	 * @param dead Deadzone of an axis
	 * @param sensitivity The speed of a Button type of input responds, or the scale factor of a Movement type of input
	 * @param invert Invert the axis/button
	 * @param type Button: any kind of button (keyboard, joystick, mouse); Movement: mouse movement; Axis: axis on a joystick
	 * @param axis The hardware axis if the type is not Button
	 * @param joyNum The number of the joystick
	 */
	void RegisterInput(std::string name, std::string posButton, std::string negButton, std::string altPosButton, std::string altNegButton, float gravity, float dead, float sensitivity, bool invert, InputType type, DSAxisCode axis, int joyNum);

private:
	/**
	 * @brief The XInput Framework reference
	 *
	 */
	DSFXInput xInput;
	/**
	 * @brief The Raw Input Framework reference
	 *
	 */
	DSFRawInput rawInput;

	/**
	 * @brief All registered inputs
	 * 
	 */
	std::list<DSSInputAxis> inputs;
};

/**
 * @brief The pointer that points to the singleton
 * of the Input System
 */
extern DSENGINESYSTEM_API DSSInput* SInput;