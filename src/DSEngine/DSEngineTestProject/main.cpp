#include <iostream>
#include "DSSTestSystem.h"

int main()
{
	// This is a test of calling a class defined in DSEngineSystem.
	std::cout << DSSTestSystem::Add(5, 10) << std::endl;
	// This is a test of calling a function defined in DSEngineSystem.
	std::cout << Subtract(20, 4) << std::endl;
}
