#pragma once
#include "Component.hpp"
#include "WheelCollider.hpp"


class WheelController :
	public Component
{
public:
	explicit WheelController(Object* owner);
	~WheelController();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;
};

