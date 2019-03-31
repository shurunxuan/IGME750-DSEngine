#pragma once
#pragma warning(disable:4251)
#include <DirectXMath.h>
#include "Collider.hpp"

class RaycastHit
{
public:
	RaycastHit();
	~RaycastHit();
	void SetCollider(BoxCollider* other);
	BoxCollider* GetCollider();

private:
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

