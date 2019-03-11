#pragma once
#pragma warning(disable:4251)
#include <string>
#include <DirectXMath.h>
#include "DSFLogging.h"
#include "Object.hpp"

class Camera final
	: public Object
{
public:
	Camera(Scene* owner, std::string name = "MainCamera");
	~Camera();

	void Update(float deltaXt, float deltaYt, float deltaZt, float deltaXr, float deltaYr);
	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float width, float height, float fov);

	DirectX::XMMATRIX& GetViewMatrix();
	DirectX::XMMATRIX& GetProjectionMatrix();

	DirectX::XMFLOAT3 GetForward() const;
	DirectX::XMFLOAT3 GetRight() const;

	DirectX::XMFLOAT3 GetPosition() const;

	float GetNearClip() const;
	float GetFarClip() const;

private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 direction;

	float xRotation;
	float yRotation;

	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;
};

inline Camera::Camera(Scene *owner, std::string name)
	: Object(owner, name)
{
	xRotation = 0.0f;
	yRotation = 0.0f;

	position = DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f);

	DirectX::XMFLOAT3 z(0.0f, 0.0f, 1.0f);
	const DirectX::XMVECTOR rotationQua = DirectX::XMQuaternionRotationRollPitchYaw(yRotation, xRotation, 0.0f);
	const DirectX::XMVECTOR newDirection = DirectX::XMVector3Rotate(XMLoadFloat3(&z), rotationQua);
	XMStoreFloat3(&direction, newDirection);
	//UpdateViewMatrix();

	//UpdateProjectionMatrix(screenWidth, screenHeight, 3.14159265f / 4.0f);

	LOG_INFO << "Camera created at <0x" << this << "> by " << __FUNCTION__ << ".";
}


inline Camera::~Camera()
{
	LOG_INFO << "Camera destroyed at <0x" << this << ">.";
}

inline void Camera::Update(float deltaXt, float deltaYt, float deltaZt, float deltaXr, float deltaYr)
{
	// Rotate direction
	xRotation += deltaXr;
	//if (xRotation > 3.1415927f * 2) xRotation -= 3.1415927f * 2;
	//else if (xRotation < 0) xRotation += 3.1415927f * 2;
	yRotation += deltaYr;
	if (yRotation > 80.0f / 180 * 3.1415927f) yRotation = 80.0f / 180 * 3.1415927f;
	else if (yRotation < -80.0f / 180 * 3.1415927f) yRotation = -80.0f / 180 * 3.1415927f;

	DirectX::XMFLOAT3 z(0.0f, 0.0f, 1.0f);
	const DirectX::XMVECTOR rotationQua = DirectX::XMQuaternionRotationRollPitchYaw(yRotation, xRotation, 0.0f);
	const DirectX::XMVECTOR newDirection = DirectX::XMVector3Rotate(XMLoadFloat3(&z), rotationQua);
	XMStoreFloat3(&direction, newDirection);

	// Translate position
	DirectX::XMFLOAT3 translate(deltaXt, deltaYt, deltaZt);
	const DirectX::XMVECTOR positionVec = XMLoadFloat3(&translate);
	const DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(XMLoadFloat3(&position), positionVec);
	XMStoreFloat3(&position, newPosition);
}

inline void Camera::UpdateViewMatrix()
{
	// 4D Vectors
	DirectX::XMFLOAT4 position4(position.x, position.y, position.z, 0.0f);
	DirectX::XMFLOAT4 direction4(direction.x, direction.y, direction.z, 0.0f);
	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	const DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 0);
	const DirectX::XMMATRIX V = DirectX::XMMatrixLookToLH(
		XMLoadFloat4(&position4),     // The position of the "camera"
		XMLoadFloat4(&direction4),     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	viewMatrix = XMMatrixTranspose(V); // Transpose for HLSL!
}

inline void Camera::UpdateProjectionMatrix(float width, float height, float fov)
{
	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(
		fov,		// Field of View Angle
		float(width) / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		1000.0f);					// Far clip plane distance
	projectionMatrix = XMMatrixTranspose(P); // Transpose for HLSL!
}

inline DirectX::XMMATRIX& Camera::GetViewMatrix()
{
	return viewMatrix;
}

inline DirectX::XMMATRIX& Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

inline DirectX::XMFLOAT3 Camera::GetForward() const
{
	const DirectX::XMVECTOR normForward = DirectX::XMVector3Normalize(XMLoadFloat3(&direction));
	DirectX::XMFLOAT3 result{};
	XMStoreFloat3(&result, normForward);
	return result;
}

inline DirectX::XMFLOAT3 Camera::GetRight() const
{
	DirectX::XMFLOAT3 up(0.0f, 1.0f, 0.0f);
	const DirectX::XMVECTOR upVec = XMLoadFloat3(&up);
	const DirectX::XMVECTOR forwardVec = XMLoadFloat3(&direction);

	const DirectX::XMVECTOR rightVec = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(upVec, forwardVec));

	DirectX::XMFLOAT3 right{};
	XMStoreFloat3(&right, rightVec);

	return right;
}

inline DirectX::XMFLOAT3 Camera::GetPosition() const
{
	return position;
}

inline float Camera::GetNearClip() const
{
	return 0.1f;
}

inline float Camera::GetFarClip() const
{
	return 1000.0f;
}
