#pragma once
#pragma warning(disable:4251)
#include <DirectXCollision.h>
#include "Component.hpp"

using namespace DirectX;

class Plane : public Component
{
public:
	Plane(Object* owner);
	~Plane();
	void Start() override;

	void Update(float deltaTime, float totalTime) override final;
private:
	
};

inline Plane::Plane(Object* owner)
	:Component(owner)
{
}

inline Plane::~Plane()
{
}

inline void Plane::Start()
{
}

inline void Plane::Update(float deltaTime, float totalTime)
{
}
