#include <DirectXMath.h>
#include "MoveObject.h"
#include "Object.hpp"
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
}
