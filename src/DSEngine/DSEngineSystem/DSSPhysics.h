#pragma once

#ifdef DSENGINESYSTEM_EXPORTS
#define DSENGINESYSTEM_API __declspec(dllexport)
#else
#define DSENGINESYSTEM_API __declspec(dllimport)
#endif

#include "Object.hpp"

class DSSPhysics
{
public:
	DSSPhysics();
	~DSSPhysics();
	void Update(float deltaTime, float totalTime);
	void HandleCollision(float deltaTime, float totalTime);
	void Simulate(float deltaTime, float totalTime);

private:

};

