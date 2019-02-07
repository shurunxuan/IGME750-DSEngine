/**
* @file DSSTestSystem.cpp
* @brief This file contains the
* definitions of class DSSTestSystem
*
* @author Victor Shu
*
* @date 2019/02/06
*/


#include "DSSTestSystem.h"
#include "DSFTestFramework.h"

DSSTestSystem::DSSTestSystem()
{
}


DSSTestSystem::~DSSTestSystem()
{
}

int DSSTestSystem::Add(int a, int b)
{
	// This is a test of calling a class defined in DSEngineFramework.
	DSFTestFramework testFramework(a, b);
	return testFramework.OutputAddResult();
}

int Subtract(int a, int b)
{
	return a - b;
}
