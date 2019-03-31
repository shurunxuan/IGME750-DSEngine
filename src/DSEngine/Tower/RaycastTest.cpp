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
	DirectX::XMVECTOR worldPos = object->GetScene()->mainCamera->ScreenToWorldPoint(mousePos_x, mousePos_y);
	DirectX::XMVECTOR cameraPos = object->GetScene()->mainCamera->transform->GetLocalTranslation();
	DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(worldPos, cameraPos);
	diff = DirectX::XMVectorScale(diff, 100.0f);
	worldPos = DirectX::XMVectorAdd(cameraPos, diff);
	DirectX::XMFLOAT3 rayOrigin;
	DirectX::XMStoreFloat3(&rayOrigin, cameraPos);
	DirectX::XMFLOAT3 rayDirection;
	DirectX::XMStoreFloat3(&rayDirection, worldPos);
	Ray my_ray = Ray(rayOrigin, rayDirection);
	LOG_INFO << rayOrigin.x << " " << rayOrigin.y << " " << rayOrigin.z;
	RaycastHit mHit = RaycastHit();
	if (SPhysics->Raycast(my_ray, mHit)) {
		LOG_INFO << mHit.GetCollider()->object->name;

	}


}
