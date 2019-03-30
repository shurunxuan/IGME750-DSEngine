#pragma once
#pragma warning(disable:4251)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <DirectXCollision.h>
#include "Component.hpp"

using namespace DirectX;

class RigidBody : public Component
{
public:
	RigidBody(Object* owner);
	~RigidBody();
	void Start();
	void Update(float deltaTime, float totalTime);
	void SetPosition(float x, float y, float z);
	void SetPosition(DirectX::XMFLOAT3 p);
	void Translate(float x, float y, float z);
	void Translate(DirectX::XMFLOAT3 t);
	void SetRotation(float x, float y, float z);
	void SetRotation(DirectX::XMFLOAT3 r);
	void Rotate(float x, float y, float z);
	void Rotate(DirectX::XMFLOAT3 r);
	void SetScale(float x, float y, float z);
	void SetScale(DirectX::XMFLOAT3 s);
	void SetDirection(float x, float y, float z);
	void SetMass(float m);
	void SetInertia(float i);
	void SetWorldMatrix();
	void SetColliderMatrix();
	void SetVelocity(float x, float y, float z);
	void SetVelocity(DirectX::XMFLOAT3 v);
	void SetAngularVelocity(float x, float y, float z);
	void SetAngularVelocity(DirectX::XMFLOAT3 f);
	void AddForce(float x, float y, float z);
	void SetForce(float x, float y, float z);
	void AddForce(DirectX::XMFLOAT3 f);
	void CalculateVelocity(float deltaTime, float totalTime);
	void CalculateWorldMatrix(float deltaTime, float totalTime);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMVECTOR GetRotationQuaterinion();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMFLOAT3 GetColliderPosition();
	DirectX::XMFLOAT3 GetColliderRotation();
	DirectX::XMFLOAT3 GetColliderScale();
	DirectX::XMFLOAT3 GetVelocity();
	DirectX::XMFLOAT3 GetDirection();
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT4X4 GetColliderWorldMatrix();
	
	float GetMass();

private:
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 colliderWorldMatrix;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 colliderPosition;
	DirectX::XMFLOAT3 colliderRotation;
	DirectX::XMFLOAT3 colliderScale;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT3 angularVelocity;
	DirectX::XMFLOAT3 force;
	DirectX::XMFLOAT3 direction;
	float inertia;
	float mass;
	float accleration;
	
};

inline RigidBody::RigidBody(Object* owner) : Component(owner) {
	position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	colliderPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	colliderScale = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	colliderRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	inertia = 0.0f;
	mass = 1.0f;
	accleration = 0.0f;
	velocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	direction = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	angularVelocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	force = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->SetWorldMatrix();
}

inline RigidBody::~RigidBody() {

}

inline void RigidBody::Start()
{
	/*position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	colliderPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	colliderScale = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	colliderRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	mass = 1.0f;
	velocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->SetWorldMatrix();*/
}

inline void RigidBody::Update(float deltaTime, float totalTime)
{
}

inline void RigidBody::SetPosition(float x, float y, float z)
{
	position = DirectX::XMFLOAT3(x, y, z);
}

inline void RigidBody::SetPosition(DirectX::XMFLOAT3 p)
{
	position =  p;
}

inline void RigidBody::Translate(float x, float y, float z)
{
	XMFLOAT3 temp = XMFLOAT3(x, y, z);
	XMVECTOR  p = XMLoadFloat3(&temp);
	XMVECTOR my_p = XMLoadFloat3(&position);
	
	my_p = XMVectorAdd(my_p, p);
	
	XMStoreFloat3(&position, my_p);

	colliderPosition = temp;
}

inline void RigidBody::Translate(DirectX::XMFLOAT3 t)
{
	
}

inline void RigidBody::SetRotation(float x, float y, float z)
{
	rotation = DirectX::XMFLOAT3(x, y, z);
}

inline void RigidBody::SetRotation(DirectX::XMFLOAT3 r)
{
	rotation = r;
}

inline void RigidBody::Rotate(float x, float y, float z)
{
	/*XMFLOAT3 temp = XMFLOAT3(x, y, z);
	XMVECTOR  r = XMLoadFloat3(&temp);
	XMVECTOR my_r = XMLoadFloat3(&rotation);
	my_r = XMVector3Rotate(my_r, r);
	XMStoreFloat3(&rotation, my_r);
	colliderRotation = temp;*/
	XMFLOAT3 temp = XMFLOAT3(x, y, z);
	XMFLOAT3 my_r = XMFLOAT3(x + rotation.x, y + rotation.y, z + rotation.z);
	rotation = my_r;
	colliderRotation = temp;

}

inline void RigidBody::Rotate(DirectX::XMFLOAT3 r)
{
	
}

inline void RigidBody::SetScale(float x, float y, float z)
{
	scale = XMFLOAT3(x, y, z);
}

inline void RigidBody::SetScale(DirectX::XMFLOAT3 s)
{
	scale = s;
}

inline void RigidBody::SetDirection(float x, float y, float z)
{
	direction = XMFLOAT3(x, y, z);
}

inline void RigidBody::SetMass(float m)
{
	mass = m;
}

inline void RigidBody::SetInertia(float i)
{
	inertia = i;
}

inline void RigidBody::SetWorldMatrix()
{
	XMVECTOR my_p = XMLoadFloat3(&position);
	XMVECTOR my_r = XMLoadFloat3(&rotation);
	XMVECTOR my_s = XMLoadFloat3(&scale);

	XMMATRIX p = XMMatrixTranslationFromVector(my_p);
	XMMATRIX r = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYawFromVector(my_r));
	XMMATRIX s = XMMatrixScalingFromVector(my_s);

	XMMATRIX temp= s * r * p;
	XMStoreFloat4x4(&worldMatrix, temp);

}

inline void RigidBody::SetColliderMatrix()
{
	XMVECTOR my_p = XMLoadFloat3(&colliderPosition);
	XMVECTOR my_r = XMLoadFloat3(&colliderRotation);
	XMVECTOR my_s = XMLoadFloat3(&colliderScale);

	XMMATRIX p = XMMatrixTranslationFromVector(my_p);
	XMMATRIX r = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYawFromVector(my_r));
	XMMATRIX s = XMMatrixScalingFromVector(my_s);

	XMMATRIX temp = s * r * p;
	XMStoreFloat4x4(&colliderWorldMatrix, temp);
}

inline void RigidBody::SetVelocity(float x, float y, float z)
{
	velocity = XMFLOAT3(x, y, z);
}

inline void RigidBody::SetVelocity(DirectX::XMFLOAT3 v)
{
	velocity = v;
}

inline void RigidBody::SetAngularVelocity(float x, float y, float z)
{
	angularVelocity = XMFLOAT3(x, y, z);
}

inline void RigidBody::SetAngularVelocity(DirectX::XMFLOAT3 f)
{
	angularVelocity = f;
}

inline void RigidBody::AddForce(float x, float y, float z)
{
	XMFLOAT3 force_1 = XMFLOAT3(force.x + x, force.y + y,force.z + z);
	force = force_1;
}

inline void RigidBody::SetForce(float x, float y, float z)
{
	XMFLOAT3 force_1 = XMFLOAT3(x, y, z);
	force = force_1;
}

inline void RigidBody::AddForce(DirectX::XMFLOAT3 f)
{
	force = f;
}

inline void RigidBody::CalculateVelocity(float deltaTime, float totalTime)
{
	float vel_x = velocity.x + force.x / mass * deltaTime;
	float vel_y = velocity.y + force.y / mass * deltaTime;
	float vel_z = velocity.z + force.z / mass * deltaTime;
	SetVelocity(vel_x, vel_y, vel_z);
	float ang_x = angularVelocity.x + inertia * force.z / mass * deltaTime / 3.1415926f *15 ;
	float ang_y = angularVelocity.y + inertia * force.y / mass * deltaTime / 3.1415926f *15 ;
	float ang_z = angularVelocity.z + inertia * force.x / mass * deltaTime / 3.1415926f *15 ;
	SetAngularVelocity(ang_x, ang_y, ang_z);
	float dir_x;
	float dir_y;
	float dir_z;

	if (vel_x > 0) dir_x = 1;
	else if (vel_x < 0) dir_x = -1;
	else dir_x = 0;

	if (vel_y > 0) dir_y = 1;
	else if (vel_y < 0) dir_y = -1;
	else dir_y = 0;

	if (vel_z > 0) dir_z = 1;
	else if (vel_z < 0) dir_z = -1;
	else dir_z = 0;

	SetDirection(dir_x, dir_y, dir_z);

}

inline void RigidBody::CalculateWorldMatrix(float deltaTime, float totalTime)
{
	CalculateVelocity(deltaTime, totalTime);
	this->Translate(deltaTime * velocity.x, deltaTime * velocity.y, deltaTime * velocity.z);
	this->Rotate(deltaTime * angularVelocity.x, 0.0f, -deltaTime * angularVelocity.z);
	this->SetScale(1.0f, 1.0f, 1.0f);
	//this->SetRotation(deltaTime, deltaTime, deltaTime);
	this->SetWorldMatrix();
	this->SetColliderMatrix();
}

inline DirectX::XMFLOAT3 RigidBody::GetPosition()
{
	return position;
}

inline DirectX::XMFLOAT3 RigidBody::GetRotation()
{
	return rotation;
}

inline DirectX::XMVECTOR RigidBody::GetRotationQuaterinion()
{
	return DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
}

inline DirectX::XMFLOAT3 RigidBody::GetScale()
{
	return scale;
}

inline DirectX::XMFLOAT3 RigidBody::GetColliderPosition()
{
	return colliderPosition;
}

inline DirectX::XMFLOAT3 RigidBody::GetColliderRotation()
{
	return colliderRotation;
}

inline DirectX::XMFLOAT3 RigidBody::GetColliderScale()
{
	return colliderScale;
}

inline float RigidBody::GetMass()
{
	return mass;
}

inline DirectX::XMFLOAT4X4 RigidBody::GetWorldMatrix()
{
	return worldMatrix;
}

inline DirectX::XMFLOAT4X4 RigidBody::GetColliderWorldMatrix()
{
	return colliderWorldMatrix;
}

inline DirectX::XMFLOAT3 RigidBody::GetVelocity()
{
	return velocity;
}

inline DirectX::XMFLOAT3 RigidBody::GetDirection()
{
	return direction;
}



