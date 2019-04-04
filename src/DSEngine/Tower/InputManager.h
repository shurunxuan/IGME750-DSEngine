#pragma once
#include "Component.hpp"

class InputManager :
	public Component
{
public:
	explicit InputManager(Object* owner);
	~InputManager();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;
	
};
