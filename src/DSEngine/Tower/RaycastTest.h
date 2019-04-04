#pragma once
#include "Component.hpp"


class RaycastTest :
	public Component
{
public:
	explicit RaycastTest(Object* owner);
	~RaycastTest();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

};

