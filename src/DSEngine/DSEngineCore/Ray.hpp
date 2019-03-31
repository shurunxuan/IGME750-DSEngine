#pragma once
#pragma warning(disable:4251)
#include <DirectXMath.h>



class Ray 
{
public:
	Ray();
	Ray(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 direction);
	DirectX::XMFLOAT3 GetOrigin();
	DirectX::XMFLOAT3 GetDirection();
	~Ray();

private:
	DirectX::XMFLOAT3 m_origin;
	DirectX::XMFLOAT3 m_direction;
};

inline Ray::Ray()
{
	m_origin = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_direction = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
}

inline Ray::Ray(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 direction)
{
	m_origin = origin;
	m_direction = direction;
}

inline DirectX::XMFLOAT3 Ray::GetOrigin()
{
	return m_origin;
}

inline DirectX::XMFLOAT3 Ray::GetDirection()
{
	return m_direction;
}


inline Ray::~Ray()
{
}
