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

#ifdef DSENGINEFRAMEWORK_EXPORTS
#define DSENGINEFRAMEWORK_API __declspec(dllexport)
#else
#define DSENGINEFRAMEWORK_API __declspec(dllimport)
#endif

class DSFXInput
{
public:
	DSFXInput();
	~DSFXInput();

	void Update();

private:
	XINPUT_STATE state[XUSER_MAX_COUNT]{};
	bool connected[XUSER_MAX_COUNT]{};
};

