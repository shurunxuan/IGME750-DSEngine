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

enum DSButtonCode
{
	DPAD_UP = 0x0001,
	DPAD_DOWN = 0x0002,
	DPAD_LEFT = 0x0004,
	DPAD_RIGHT = 0x0008,
	START = 0x0010,
	BACK = 0x0020,
	LEFT_THUMB = 0x0040,
	RIGHT_THUMB = 0x0080,
	LEFT_SHOULDER = 0x0100,
	RIGHT_SHOULDER = 0x0200,
	RESERVED_0 = 0x0400,
	RESERVED_1 = 0x0800,
	A = 0x1000,
	B = 0x2000,
	X = 0x4000,
	Y = 0x8000
};

enum DSAxisCode
{
	LT,
	RT,
	LX,
	LY,
	RX,
	RY,
	ERROR_AXIS
};

class DSENGINEFRAMEWORK_API DSFXInput
{
public:
	DSFXInput();
	~DSFXInput();

	void Init();
	void Update();

	bool GetButton(DSButtonCode buttonCode, int player) const;
	bool GetButtonDown(DSButtonCode buttonCode, int player) const;
	bool GetButtonUp(DSButtonCode buttonCode, int player) const;
	WORD GetDownEvent(int player) const;
	WORD GetUpEvent(int player) const;
	float GetAxis(DSAxisCode axisCode, int player, float deadZone = -1) const;
	float GetRawAxis(DSAxisCode axisCode, int player) const;

	static DSButtonCode GetButtonCode(unsigned int code);
	static DSAxisCode GetAxisCode(unsigned int code);
	static std::string GetButtonName(DSButtonCode buttonCode);
	static std::string GetAxisName(DSAxisCode axisCode);

private:
	XINPUT_STATE buttonState[XUSER_MAX_COUNT]{};
	XINPUT_STATE buttonDownState[XUSER_MAX_COUNT]{};
	XINPUT_STATE buttonUpState[XUSER_MAX_COUNT]{};

	XINPUT_STATE lastState[XUSER_MAX_COUNT]{};
	bool connected[XUSER_MAX_COUNT]{};
};

