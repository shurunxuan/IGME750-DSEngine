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

#ifdef DSENGINESYSTEM_EXPORTS
#define DSENGINESYSTEM_API __declspec(dllexport)
#else
#define DSENGINESYSTEM_API __declspec(dllimport)
#endif

/**
 * @brief The Input System of the DS Engine
 * 
 */
class DSENGINESYSTEM_API DSSInput
{
public:
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
	 */
	void Init();
	/**
	 * @brief Update the input status
	 * 
	 */
	void Update();

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
};

/**
 * @brief The pointer that points to the singleton
 * of the Input System
 */
extern DSENGINESYSTEM_API DSSInput* SInput;