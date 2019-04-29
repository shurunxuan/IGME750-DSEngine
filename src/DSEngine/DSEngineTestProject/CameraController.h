#pragma once
#include "Component.hpp"
#include "Transform.hpp"

/**
 * @brief Control the camera with Left & Right Stick
 * 
 */
class CameraController :
	public Component
{
public:
	explicit CameraController(Object* owner);
	~CameraController();

	void Start() override;
	void Update(float deltaTime, float totalTime) override;

private:
};

