/**
* @file DSFTestFramework.h
* @brief This header file contains the 
* declarations of class DSFTestFramework
*
* @author Victor Shu
*
* @date 2019/02/06
*/

#pragma once

#ifdef DSENGINEFRAMEWORK_EXPORTS
#define DSENGINEFRAMEWORK_API __declspec(dllexport)
#else
#define DSENGINEFRAMEWORK_API __declspec(dllimport)
#endif

/**
 * @class DSFTestFramework
 *
 * @brief This is a test framework class for
 * DS Engine systems.
 *
 * This is a test system framework for DS Engine
 * systems. Consider it provides a fundamental 
 * functionality of adding two numbers.
 */
class DSENGINEFRAMEWORK_API DSFTestFramework
{
public:
	/**
	 * Constructor. 
	 * 
	 * @param a Set the aVal member of the class.
	 * @param b Set the bVal member of the class.
	 */
	DSFTestFramework(int a, int b);
	/**
	 * Default destructor. Does nothing.
	 */
	~DSFTestFramework();

	/**
	 * A simple function that just add the two member
	 * variables.
	 * 
	 * @return aVal + bVal
	 */
	int OutputAddResult() const;

private:
	/**
	 * A member variable.
	 */
	int aVal;
	/**
	 * Another member variable.
	 */
	int bVal;
};

