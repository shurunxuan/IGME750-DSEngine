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

#ifdef DSENGINESYSTEM_EXPORTS
#define DSENGINESYSTEM_API __declspec(dllexport)
#else
#define DSENGINESYSTEM_API __declspec(dllimport)
#endif

class DSENGINESYSTEM_API DSSInput
{
public:
	DSSInput();
	~DSSInput();

	void Init();
	void Update();

private:
	DSFXInput xInput;
};

