/**
 * @file Ray.hpp
 * @author Weihao Yan
 * @brief This file contains the class that represents
 * the Ray
 *
 * @version 0.1
 * @date 2019/04/03
 *
 * @copyright Copyright (c) 2019
 *
 */

#pragma once
#pragma warning(disable:4251)
#include <DirectXMath.h>



class Ray 
{
public:
	/**
	 * @brief Default Construct a new Ray
	 *
	 */
	Ray();

	/**
	 * @brief Construct a new Ray
	 *
	 * @param origin The origin of this Ray
	 * @param target The origin of this Ray
	 */
	Ray(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 target);

	/**
	 * @brief Get the origin of this Ray
	 *
	 */
	DirectX::XMFLOAT3 GetOrigin();

	/**
	 * @brief Get the target of this Ray
	 *
	 */
	DirectX::XMFLOAT3 GetTarget();

	/**
	 * @brief Destroy the Ray
	 *
	 */
	~Ray();

private:
	/**
	 * @brief The origin of this Ray
	 *
	 */
	DirectX::XMFLOAT3 m_origin;

	/**
	 * @brief The target of this Ray
	 *
	 */
	DirectX::XMFLOAT3 m_target;
};

inline Ray::Ray()
{
	m_origin = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_target = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

inline Ray::Ray(DirectX::XMFLOAT3 origin, DirectX::XMFLOAT3 target)
{
	m_origin = origin;
	m_target = target;
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
