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

enum DSENGINESYSTEM_API InputType
{
	Button,
	Movement,
	Axis
};

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

class DSENGINESYSTEM_API DSSInputAxis
{
public:
	friend class DSSInput;

private:
	std::string name;
	std::string posButton;
	std::string negButton;
	std::string altPosButton;
	std::string altNegButton;
	float gravity;
	float dead;
	float sensitivity;
	bool invert;
	InputType type;
	DSAxisCode axis;
	int joyNum;

	DSSInput* inputSystem;

	float processedAxisValue;

	// 1: pos, 2: altPos, 3: neg, 4: altNeg
	std::function<bool()> getButton[4];
	std::function<bool()> getButtonDown[4];
	std::function<bool()> getButtonUp[4];

	std::function<float()> getRawAxis;

	void BindButtonCallback(const std::string& buttonName, int buttonFuncIndex);
	void BindCallbackFunctions();

	void Update(float deltaTime);

public:
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
	 * @brief Update the input status
	 *
	 * @param deltaTime The time between two DSSInput updates
	 */
	void Update(float deltaTime);

	bool GetButton(const std::string& name);
	bool GetButtonDown(const std::string& name);
	bool GetButtonUp(const std::string& name);

	float GetAxis(const std::string& name);
	float GetRawAxis(const std::string& name);

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

	std::list<DSSInputAxis> inputs;
};

/**
 * @brief The pointer that points to the singleton
 * of the Input System
 */
extern DSENGINESYSTEM_API DSSInput* SInput;