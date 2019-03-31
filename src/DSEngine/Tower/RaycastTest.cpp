#include "RaycastTest.h"
#include "Object.hpp"
#include "Ray.hpp"
#include "RaycastHit.hpp"
#include "DSSInput.h"
#include "DSSPhysics.h"
#include "Scene.hpp"



RaycastTest::RaycastTest(Object* owner)
	:Component(owner)
{
}


RaycastTest::~RaycastTest()
{
}

void RaycastTest::Start()
{
}

void RaycastTest::Update(float deltaTime, float totalTime)
{
	float mousePos_x;
	float mousePos_y;
	SInput->GetMousePosition(&mousePos_x, &mousePos_y);
	DirectX::XMFLOAT3 rayOrigin = DirectX::XMFLOAT3(mousePos_x, mousePos_y, 0.0f);
	DirectX::XMFLOAT3 rayDirection = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	Ray my_ray = Ray(rayOrigin, rayDirection);
	RaycastHit mHit = RaycastHit();
	if (SPhysics->Raycast(my_ray, mHit)) {
		mHit.GetCollider()->object->GetScene()->DestroyObject(mHit.GetCollider()->object);
	}
}
