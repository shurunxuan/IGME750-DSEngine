/**
* @file DSFTestFramework.cpp
* @brief This file contains the
* definitions of class DSFTestFramework
*
* @author Victor Shu
*
* @date 2019/02/06
*/

#include "DSFTestFramework.h"



DSFTestFramework::DSFTestFramework(int a, int b)
{
	aVal = a;
	bVal = b;
}


DSFTestFramework::~DSFTestFramework()
{
}

int DSFTestFramework::OutputAddResult() const
{
	return aVal + bVal;
}
