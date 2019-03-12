#pragma once
#include "Component.hpp"


class MoveParentObject :
	public Component
{
public:
	explicit MoveParentObject(Object* owner);
	~MoveParentObject();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;
};

