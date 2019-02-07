#pragma once

#ifdef DSENGINESYSTEM_EXPORTS
#define DSENGINESYSTEM_API __declspec(dllexport)
#else
#define DSENGINESYSTEM_API __declspec(dllimport)
#endif

class DSENGINESYSTEM_API DSSTestSystem
{
public:
	DSSTestSystem();
	~DSSTestSystem();

	static int Add(int a, int b);
};

int DSENGINESYSTEM_API Subtract(int a, int b);

