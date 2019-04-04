/**
 * @file Camera.hpp
 * @author Victor Shu
 * @brief This file contains the class that represents
 * the camera object
 * 
 * @version 0.1
 * @date 2019/03/11
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once
#pragma warning(disable:4251)
#include <string>
#include <DirectXMath.h>
#include "DSFLogging.h"
#include "Object.hpp"
#include "Skybox.hpp"

/**
 * @brief Camera
 * 
 */
class Camera final
	: public Object
{
public:
	/**
	 * @brief Construct a new Camera object
	 * 
	 * @param owner The scene that owns the camera
	 * @param name The name of the camera, default to MainCamera
	 */
	Camera(Scene* owner, std::string name = "MainCamera");
	/**
	 * @brief Destroy the Camera object
	 * 
	 */
	~Camera();

	/**
	 * @brief Create a skybox from file
	 * 
	 * @param d Direct3D 11 device
	 * @param c Direct3D 11 device context
	 * @param cubeMapFile The cubemap of the skybox
	 * @param irradianceMapFile The irradiance map of the cubemap
	 */
	void SetSkybox(ID3D11Device* d, ID3D11DeviceContext* c, const std::wstring& cubeMapFile, const std::wstring& irradianceMapFile);
	/**
	 * @brief Get the Skybox object
	 * 
	 * @return Skybox* A pointer points to the skybox
	 */
	Skybox* GetSkybox();

	/**
	 * @brief Update the projection matrix
	 * 
	 * @param width The screen width
	 * @param height The screen height
	 * @param fov The field of view
	 */
	void UpdateProjectionMatrix(float width, float height, float fov);

	/**
	 * @brief Get the projection matrix
	 * 
	 * @return DirectX::XMMATRIX& The reference of the projection matrix 
	 */
	DirectX::XMMATRIX& GetProjectionMatrix();

	/**
	 * @brief Get the View matrix
	 *
	 * @return DirectX::XMMATRIX The reference of the projection matrix
	 */
	DirectX::XMMATRIX GetViewMatrix() const;

	/**
	 * @brief Get the Near Clip of the camera
	 * 
	 * @return float Near clip
	 * 
	 * @todo Currently, this only returns 0.1f
	 */
	float GetNearClip() const;
	/**
	 * @brief Get the Far Clip of the camera
	 * 
	 * @return float Far clip
	 * 
	 * @todo Currently, this only returns 1000.0f
	 */
	float GetFarClip() const;

	/**
	 * @brief Get the world point from mouse cursor position (near plane in Z)
	 * 
	 * @param x X position of the mouse cursor in screen
	 * @param y Y position of the mouse cursor in screen
	 * 
	 * @return DirectX::XMVECTOR The world position
	 */
	DirectX::XMVECTOR ScreenToWorldPoint(float x, float y) const;

	//DirectX::XMFLOAT2 WorldPositionToScreenPoint(DirectX::XMVECTOR worldPosition) const;

private:
	/**
	 * @brief The projection matrix
	 * 
	 */
	DirectX::XMMATRIX projectionMatrix;
	/**
	 * @brief The skybox
	 * 
	 */
	Skybox* skybox;

	/**
	 * @brief Client width
	 */
	float width;

	/**
	 * @brief Client height
	 */
	float height;
};

inline Camera::Camera(Scene *owner, std::string name)
	: Object(owner, name)
{
	skybox = nullptr;
	//UpdateViewMatrix();

	//UpdateProjectionMatrix(screenWidth, screenHeight, 3.14159265f / 4.0f);
}


inline Camera::~Camera()
{
	delete skybox;
}

inline void Camera::SetSkybox(ID3D11Device* d, ID3D11DeviceContext* c, const std::wstring& cubeMapFile,
	const std::wstring& irradianceMapFile = L"")
{
	delete skybox;
	skybox = new Skybox(d, c, cubeMapFile, irradianceMapFile);
}

inline Skybox* Camera::GetSkybox()
{
	return skybox;
}

inline void Camera::UpdateProjectionMatrix(float width, float height, float fov)
{
	this->width = width;
	this->height = height;
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

inline DirectX::XMMATRIX& Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

inline DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	return XMMatrixTranspose(DirectX::XMMatrixLookToLH(transform->GetGlobalTranslation(), transform->Forward(), transform->Up()));
}

inline float Camera::GetNearClip() const
{
	return 0.1f;
}

inline float Camera::GetFarClip() const
{
	return 1000.0f;
}

inline DirectX::XMVECTOR Camera::ScreenToWorldPoint(float x, float y) const
{
	const float ndcX = 2.0f * x / width - 1;
	const float ndcY = -2.0f * y / height + 1;
	const DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(XMMatrixTranspose(GetViewMatrix()), XMMatrixTranspose(projectionMatrix));
	const DirectX::XMMATRIX invViewProj = XMMatrixInverse(nullptr, viewProj);
	const DirectX::XMVECTOR screenPoint = DirectX::XMVectorSet(ndcX, ndcY, 0.0f, 1.0f);
	DirectX::XMVECTOR worldPos = DirectX::XMVector3TransformCoord(screenPoint, invViewProj);
	return worldPos;

}

//inline DirectX::XMFLOAT2 Camera::WorldPositionToScreenPoint(DirectX::XMVECTOR worldPosition) const
//{
//	const DirectX::XMMATRIX view = XMMatrixInverse(nullptr, XMMatrixTranspose(transform->GetGlobalWorldMatrix()));
//	const DirectX::XMMATRIX viewProj = DirectX::XMMatrixMultiply(XMMatrixTranspose(projectionMatrix), view);
//	worldPosition = DirectX::XMVector3Transform(worldPosition, viewProj);
//	const float x = roundf(((worldPosition.m128_f32[0] + 1) / 2.0f) *
//		width);
//	const float y = roundf(((1 - worldPosition.m128_f32[1]) / 2.0f) *
//		height);
//	return {x, y};
//}

///@endcond
