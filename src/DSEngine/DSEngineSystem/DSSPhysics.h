#pragma once

#ifdef DSENGINESYSTEM_EXPORTS
#define DSENGINESYSTEM_API __declspec(dllexport)
#else
#define DSENGINESYSTEM_API __declspec(dllimport)
#endif

#include "Object.hpp"
#include "Ray.hpp"
#include "RaycastHit.hpp"

class DSENGINESYSTEM_API DSSPhysics
{
public:
	DSSPhysics();
	~DSSPhysics();
	void Update(float deltaTime, float totalTime);
	void HandleCollision(float deltaTime, float totalTime);
	void Simulate(float deltaTime, float totalTime);
	bool Raycast(Ray ray, RaycastHit &mHit);
	
private:
	std::vector<Collider*> colliders;
	std::vector<RigidBody*> rigidBodies;
	std::vector<BoxCollider*> boxColliders;
};

extern DSENGINESYSTEM_API DSSPhysics* SPhysics;
