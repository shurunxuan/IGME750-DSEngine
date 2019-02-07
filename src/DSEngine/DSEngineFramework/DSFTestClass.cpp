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
