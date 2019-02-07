/**
* @file DSSTestSystem.h
* @brief This header file contains the
* declarations of class DSSTestSystem
*
* @author Victor Shu
*
* @date 2019/02/06
*/

#pragma once

#ifdef DSENGINESYSTEM_EXPORTS
#define DSENGINESYSTEM_API __declspec(dllexport)
#else
#define DSENGINESYSTEM_API __declspec(dllimport)
#endif

/**
 * @class DSSTestSystem
 * 
 * @brief This is a test system class for 
 * DS Engine systems.
 * 
 * This is a test system class for DS Engine
 * systems. It calls the DSFTestFramework class.
 */
class DSENGINESYSTEM_API DSSTestSystem
{
public:
	/**
	 * Default constructor. Does nothing.
	 */
	DSSTestSystem();
	/**
	 * Default destructor. Does nothing.
	 */
	~DSSTestSystem();

	/**
	 * A simple function that just add the two parameters. 
	 * It calls the DSFTestFramework::OutputAddResult().
	 * 
	 * @param a An operand.
	 * @param b Another operand.
	 * @return a + b
	 */
	static int Add(int a, int b);
};

int DSENGINESYSTEM_API Subtract(int a, int b);

