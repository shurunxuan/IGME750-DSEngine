#pragma once
#include "Component.hpp"

class FreeCam :
	public Component
{
public:
	explicit FreeCam(Object* owner);
	~FreeCam();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;
};
