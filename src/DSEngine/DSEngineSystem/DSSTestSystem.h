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
	 * @brief Default constructor. Does nothing.
	 */
	DSSTestSystem();
	/**
	 * @brief Default destructor. Does nothing.
	 */
	~DSSTestSystem();

	/**
	 * @ brief A simple function that just add the two parameters. 
	 * It calls the DSFTestFramework::OutputAddResult().
	 * 
	 * @param a An operand.
	 * @param b Another operand.
	 * @return a + b
	 */
	static int Add(int a, int b);
};

/**
 * @brief A very simple subtract function that 
 * is exported for the game to use.
 * 
 * @param a An operand.
 * @param b Another operand.
 * @return int a - b 
 */
int DSENGINESYSTEM_API Subtract(int a, int b);

