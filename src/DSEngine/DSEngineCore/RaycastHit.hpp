/**
 * @file RaycastHit.hpp
 * @author Weihao Yan
 * @brief This file contains the class that represents
 * the RaycastHit
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
#include "BoxCollider.hpp"

class RaycastHit
{
public:
	/**
	 * @brief Default Construct a new RaycastHit
	 *
	 */
	RaycastHit();

	/**
	 * @brief Destory the RaycastHit
	 *
	 */
	~RaycastHit();

	/**
	 * @brief Set the BoxCollider* of this RaycastHit
	 *
	 * @param other The BoxCollider of the Object that the RaycastHit
	 */
	void SetCollider(BoxCollider* other);

	/**
	 * @brief Get the BoxCollider* of this RaycastHit
	 *
	 */
	BoxCollider* GetCollider();

private:
	/**
	 * @brief the BoxCollider* of this RaycastHit
	 *
	 */
	BoxCollider* collider;
};

inline RaycastHit::RaycastHit()
{
}

inline RaycastHit::~RaycastHit()
{
}

inline void RaycastHit::SetCollider(BoxCollider* other)
{
	collider = other;
}

inline BoxCollider* RaycastHit::GetCollider()
{
	return collider;
}

