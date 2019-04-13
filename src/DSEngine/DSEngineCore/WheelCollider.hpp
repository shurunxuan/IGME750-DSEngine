#pragma once
#pragma warning(disable:4251)
#include <DirectXCollision.h>
#include "Component.hpp"

class WheelCollider : public Component
{
public:
	explicit WheelCollider(Object* owner);
	~WheelCollider() final;
	void Start() override;
	void Update(float deltaTime, float totalTime) override final;
	void SetCenter(float x, float y, float z);
	void SetCenter(DirectX::XMFLOAT3 _center);
	void SetPosition(float x, float y, float z);
	void SetPosition(DirectX::XMFLOAT3 _position);
	void SetRotation(float x, float y, float z);
	void SetRadius(float r);
	void SetMass(float m);
	void SetSteerFactor(float factor);
	void SetMotorTorque(float mTorque);
	void SetBrakeTorque(float bTorque);
	void SetMaxSteeringAngle(float angle);
	void SetPreviousAngle(float angle);
	void SetCurrentAngle(float angle);
	void SetIsGrounded(bool _isGrounded);

	DirectX::XMFLOAT3 GetCenter();
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	float GetRadius();
	float GetMass();
	float GetSteerFactor();
	float GetMotorTorque();
	float GetBrakeTorque();
	float GetMaxSteeringAngle();
	float GetPreviousAngle();
	float GetCurrentAngle();
	bool IsGrounded();


private:
	DirectX::XMFLOAT3 center;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	float radius;
	float mass;
	float steerFactor;
	float motorTorque;
	float brakeTorque;
	float maxSteeringAngle;
	float previousAngle;
	float currentAngle;
	bool isGrounded;

};

inline WheelCollider::WheelCollider(Object * owner)
	:Component(owner)
{
	center = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	radius = 1.0f;
	mass = 50.0f;
	steerFactor = 0.0f;
	motorTorque = 0.0f;
	brakeTorque = 0.0f;
	previousAngle = 0.0f;
	currentAngle = 0.0f;
	isGrounded = false;
}

inline WheelCollider::~WheelCollider()
{
}

inline void WheelCollider::Start()
{
}

inline void WheelCollider::Update(float deltaTime, float totalTime)
{
}

inline void WheelCollider::SetCenter(float x, float y, float z)
{
	center = DirectX::XMFLOAT3(x, y, z);
}

inline void WheelCollider::SetCenter(DirectX::XMFLOAT3 _center)
{
	center = _center;
}

inline void WheelCollider::SetPosition(float x, float y, float z)
{
	position = DirectX::XMFLOAT3(x, y, z);
}

inline void WheelCollider::SetPosition(DirectX::XMFLOAT3 _position)
{
	position = _position;
}

inline void WheelCollider::SetRotation(float x, float y, float z)
{
	rotation = DirectX::XMFLOAT3(x, y, z);
}

inline void WheelCollider::SetRadius(float r)
{
	radius = r;
}

inline void WheelCollider::SetMass(float m)
{
	mass = m;
}

inline void WheelCollider::SetSteerFactor(float factor)
{
	steerFactor = factor;
}

inline void WheelCollider::SetMotorTorque(float mTorque)
{
	motorTorque = mTorque;
}

inline void WheelCollider::SetBrakeTorque(float bTorque)
{
	brakeTorque = bTorque;
}

inline void WheelCollider::SetMaxSteeringAngle(float angle)
{
	maxSteeringAngle = angle;
}

inline void WheelCollider::SetPreviousAngle(float angle)
{
	previousAngle = angle;
}

inline void WheelCollider::SetCurrentAngle(float angle)
{
	currentAngle = angle;
}

inline void WheelCollider::SetIsGrounded(bool _isGrounded)
{
	isGrounded = _isGrounded;
}

inline DirectX::XMFLOAT3 WheelCollider::GetCenter()
{
	return center;
}

inline DirectX::XMFLOAT3 WheelCollider::GetPosition()
{
	return position;
}

inline DirectX::XMFLOAT3 WheelCollider::GetRotation()
{
	return rotation;
}

inline float WheelCollider::GetRadius()
{
	return radius;
}

inline float WheelCollider::GetMass()
{
	return mass;
}

inline float WheelCollider::GetSteerFactor()
{
	return steerFactor;
}

inline float WheelCollider::GetMotorTorque()
{
	return motorTorque;
}

inline float WheelCollider::GetBrakeTorque()
{
	return brakeTorque;
}

inline float WheelCollider::GetMaxSteeringAngle()
{
	return maxSteeringAngle;
}

inline float WheelCollider::GetPreviousAngle()
{
	return previousAngle;
}

inline float WheelCollider::GetCurrentAngle()
{
	return currentAngle;
}

inline bool WheelCollider::IsGrounded()
{
	return isGrounded;
}

