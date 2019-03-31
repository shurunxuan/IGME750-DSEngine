#include <DirectXMath.h>
#include "MoveObject.h"
#include "Object.hpp"
#include "Scene.hpp"
#include "DSSInput.h"

MoveObject* MoveObject::instance = nullptr;

MoveObject::MoveObject(Object* owner)
	: Component(owner)
{
	instance = this;
}


MoveObject::~MoveObject()
{
}

void MoveObject::Start()
{
}

void MoveObject::Update(float deltaTime, float totalTime)
{
	float horizontal = SInput->GetAxis("Horizontal");
	float vertical = SInput->GetAxis("Vertical");

	DirectX::XMVECTOR position = object->transform->GetLocalTranslation();
	position = DirectX::XMVectorAdd(position, DirectX::XMVectorSet(horizontal * 4 * deltaTime, vertical * 4 * deltaTime, 0.0f, 0.0f));
	object->transform->SetLocalTranslation(position);

	if (SInput->GetButton("Test"))
	{
		float x;
		float y;
		SInput->GetMousePosition(&x, &y);
		DirectX::XMVECTOR worldPos = object->GetScene()->mainCamera->ScreenToWorldPoint(x, y);
		DirectX::XMVECTOR cameraPos = object->GetScene()->mainCamera->transform->GetLocalTranslation();
		DirectX::XMVECTOR diff = DirectX::XMVectorSubtract(worldPos, cameraPos);
		diff = DirectX::XMVectorScale(diff, 100.0f);
		worldPos = DirectX::XMVectorAdd(cameraPos, diff);
		object->transform->SetLocalTranslation(worldPos);
	}
}
