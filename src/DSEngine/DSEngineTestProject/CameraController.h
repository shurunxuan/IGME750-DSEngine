#pragma once
#include "Component.hpp"

class CameraController :
	public Component
{
public:
	explicit CameraController(Object* owner);
	~CameraController();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;
};

