#pragma once
#pragma warning(disable:4251)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <DirectXCollision.h>
#include "Component.hpp"


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
	void SetRotation(float x, float y, float z);
	void SetRotation(DirectX::XMFLOAT3 r);
	void Rotate(float x, float y, float z);
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
	void SetCollisionPoint(float x, float y, float z);
	void SetCollisionPoint(DirectX::XMFLOAT3 p);
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
	DirectX::XMFLOAT3 GetAngularVelocity();
	DirectX::XMFLOAT3 GetDirection();
	DirectX::XMFLOAT3 GetCollisionPoint();
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT4X4 GetColliderWorldMatrix();
	float GetInertia();
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
	DirectX::XMFLOAT3 collisionPoint;
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
	collisionPoint = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->SetWorldMatrix();
}

inline RigidBody::~RigidBody() {

}

inline void RigidBody::Start()
{

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
	DirectX::XMFLOAT3 temp(x, y, z);
	DirectX::XMVECTOR  p = DirectX::XMLoadFloat3(&temp);
	DirectX::XMVECTOR my_p = XMLoadFloat3(&position);
	my_p = DirectX::XMVectorAdd(my_p, p);
	DirectX::XMStoreFloat3(&position, my_p);
	colliderPosition = temp;
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
	DirectX::XMFLOAT3 temp(x, y, z);
	DirectX::XMFLOAT3 my_r(x + rotation.x, y + rotation.y, z + rotation.z);
	rotation = my_r;
	colliderRotation = temp;

}


inline void RigidBody::SetScale(float x, float y, float z)
{
	scale = DirectX::XMFLOAT3(x, y, z);
}

inline void RigidBody::SetScale(DirectX::XMFLOAT3 s)
{
	scale = s;
}

inline void RigidBody::SetDirection(float x, float y, float z)
{
	direction = DirectX::XMFLOAT3(x, y, z);
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
	DirectX::XMVECTOR my_p = XMLoadFloat3(&position);
	DirectX::XMVECTOR my_r = XMLoadFloat3(&rotation);
	DirectX::XMVECTOR my_s = XMLoadFloat3(&scale);

	DirectX::XMMATRIX p = DirectX::XMMatrixTranslationFromVector(my_p);
	DirectX::XMMATRIX r = DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(my_r));
	DirectX::XMMATRIX s = DirectX::XMMatrixScalingFromVector(my_s);

	DirectX::XMMATRIX temp= s * r * p;
	DirectX::XMStoreFloat4x4(&worldMatrix, temp);

}

inline void RigidBody::SetColliderMatrix()
{
	DirectX::XMVECTOR my_p = DirectX::XMLoadFloat3(&colliderPosition);
	DirectX::XMVECTOR my_r = DirectX::XMLoadFloat3(&colliderRotation);
	DirectX::XMVECTOR my_s = DirectX::XMLoadFloat3(&colliderScale);

	DirectX::XMMATRIX p = DirectX::XMMatrixTranslationFromVector(my_p);
	DirectX::XMMATRIX r = DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(my_r));
	DirectX::XMMATRIX s = DirectX::XMMatrixScalingFromVector(my_s);

	DirectX::XMMATRIX temp = s * r * p;
	DirectX::XMStoreFloat4x4(&colliderWorldMatrix, temp);
}

inline void RigidBody::SetVelocity(float x, float y, float z)
{
	velocity = DirectX::XMFLOAT3(x, y, z);
}

inline void RigidBody::SetVelocity(DirectX::XMFLOAT3 v)
{
	velocity = v;
}

inline void RigidBody::SetAngularVelocity(float x, float y, float z)
{
	angularVelocity = DirectX::XMFLOAT3(x, y, z);
}

inline void RigidBody::SetAngularVelocity(DirectX::XMFLOAT3 f)
{
	angularVelocity = f;
}

inline void RigidBody::AddForce(float x, float y, float z)
{
	DirectX::XMFLOAT3 force_1(force.x + x, force.y + y,force.z + z);
	force = force_1;
}

inline void RigidBody::SetForce(float x, float y, float z)
{
	DirectX::XMFLOAT3 force_1(x, y, z);
	force = force_1;
}

inline void RigidBody::SetCollisionPoint(float x, float y, float z)
{
	collisionPoint = DirectX::XMFLOAT3(x, y, z);
}

inline void RigidBody::SetCollisionPoint(DirectX::XMFLOAT3 p)
{
	collisionPoint = p;
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
	DirectX::XMMATRIX objX = DirectX::XMMatrixRotationX(rotation.x);
	DirectX::XMMATRIX objY = DirectX::XMMatrixRotationY(rotation.y);
	DirectX::XMMATRIX objZ = DirectX::XMMatrixRotationZ(rotation.z);
	return DirectX::XMQuaternionRotationMatrix(objX * objY * objZ);
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

inline float RigidBody::GetInertia()
{
	return inertia;
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

inline DirectX::XMFLOAT3 RigidBody::GetAngularVelocity()
{
	return angularVelocity;
}

inline DirectX::XMFLOAT3 RigidBody::GetDirection()
{
	return direction;
}

inline DirectX::XMFLOAT3 RigidBody::GetCollisionPoint()
{
	return collisionPoint;
}



