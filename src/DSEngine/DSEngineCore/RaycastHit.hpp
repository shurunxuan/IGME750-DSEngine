#pragma once
#pragma warning(disable:4251)
#include <DirectXMath.h>
#include "Collider.hpp"

class RaycastHit
{
public:
	RaycastHit();
	~RaycastHit();
	void SetCollider(Collider* other);
	Collider* GetCollider();

private:
	Collider* collider;
};

inline RaycastHit::RaycastHit()
{
}

inline RaycastHit::~RaycastHit()
{
}

inline void RaycastHit::SetCollider(Collider* other)
{
	collider = other;
}

inline Collider* RaycastHit::GetCollider()
{
	return collider;
}

