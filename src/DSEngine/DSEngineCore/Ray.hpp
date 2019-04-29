#pragma once
#pragma warning(disable:4251)
#include <DirectXMath.h>



class Ray
{
public:
	Ray();
	Ray(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 direction);
	DirectX::XMFLOAT3 GetOrigin();
	DirectX::XMFLOAT3 GetTarget();
	~Ray();

private:
	DirectX::XMFLOAT3 m_origin;
	DirectX::XMFLOAT3 m_target;
};

inline Ray::Ray()
{
	m_origin = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_target = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

inline Ray::Ray(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 direction)
{
	m_origin = origin;
	m_target = direction;
}

inline DirectX::XMFLOAT3 Ray::GetOrigin()
{
	return m_origin;
}

inline DirectX::XMFLOAT3 Ray::GetTarget()
{
	return m_target;
}


inline Ray::~Ray()
{
}