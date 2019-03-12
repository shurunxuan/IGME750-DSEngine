#pragma once
#pragma warning(disable:4251)
#include <string>
#include <DirectXMath.h>
#include "DSFLogging.h"
#include "Object.hpp"
#include "Skybox.hpp"

class Camera final
	: public Object
{
public:
	Camera(Scene* owner, std::string name = "MainCamera");
	~Camera();

	void SetSkybox(ID3D11Device* d, ID3D11DeviceContext* c, const std::wstring& cubeMapFile, const std::wstring& irradianceMapFile);
	Skybox* GetSkybox();

	void UpdateProjectionMatrix(float width, float height, float fov);

	DirectX::XMMATRIX& GetProjectionMatrix();

	float GetNearClip() const;
	float GetFarClip() const;

private:
	DirectX::XMMATRIX projectionMatrix;
	Skybox* skybox;
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
	const std::wstring& irradianceMapFile)
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

inline float Camera::GetNearClip() const
{
	return 0.1f;
}

inline float Camera::GetFarClip() const
{
	return 1000.0f;
}
