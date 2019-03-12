#pragma once

#include "Component.hpp"
#include "Transform.hpp"

class TestTransform :
	public Component
{
public:
	TestTransform(Object* owner);
	~TestTransform();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

private:
	Transform* parent;
	bool gotParent;
};

