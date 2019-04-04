#include "DSSPhysics.h"
#include "DSEngineApp.h"

DSSPhysics* SPhysics;

DSSPhysics::DSSPhysics()
{
	SPhysics = this;
}


DSSPhysics::~DSSPhysics()
{
}

void DSSPhysics::Update(float deltaTime, float totalTime)
{
	boxColliders.clear();

	for (Object* object : App->CurrentActiveScene()->allObjects)
	{
		if (object->GetComponent<BoxCollider>()) boxColliders.push_back(object->GetComponent<BoxCollider>());
	}
	for (int i = 0; i < boxColliders.size(); i++) {
		DirectX::BoundingBox newBox;
		newBox.Transform(newBox, 0.5f, DirectX::XMQuaternionIdentity(), boxColliders[i]->object->transform->GetLocalTranslation());
		*boxColliders[i]->GetCollider() = newBox;
	}
}

void DSSPhysics::HandleCollision(float deltaTime, float totalTime)
{
}

void DSSPhysics::Simulate(float deltaTime, float totalTime)
{
}

bool DSSPhysics::Raycast(Ray ray, RaycastHit &mHit)
{
	DirectX::XMFLOAT3 origin = ray.GetOrigin();
	DirectX::XMFLOAT3 target = ray.GetTarget();
	for (int i = 0; i < boxColliders.size(); i++) {
		DirectX::XMFLOAT3 center = boxColliders[i]->GetCollider()->Center;
		float distanceToSide = sqrt(pow(boxColliders[i]->GetCollider()->Extents.x, 2.0f) + pow(boxColliders[i]->GetCollider()->Extents.y, 2.0f) + pow(boxColliders[i]->GetCollider()->Extents.z, 2.0f));
		float originToDiretion = sqrt(pow((origin.x - target.x), 2.0f) + pow((origin.y - target.y), 2.0f) + pow((origin.z - target.z), 2.0f));
		float originToCenter = sqrt(pow((origin.x - center.x), 2.0f) + pow((origin.y - center.y), 2.0f) + pow((origin.z - center.z), 2.0f));
		float targetToCenter = sqrt(pow((target.x - center.x), 2.0f) + pow((target.y - center.y), 2.0f) + pow((target.z - center.z), 2.0f));
		float cos_A = (pow(originToCenter, 2.0f) + pow(originToDiretion, 2.0f) - pow(targetToCenter, 2.0f)) / (2.0f * originToCenter*originToDiretion);
		float sin_A = sqrt(1 - pow(cos_A, 2.0f));
		float distanceTocenter = originToCenter * sin_A;
		if (distanceTocenter <= distanceToSide) {
			mHit.SetCollider(boxColliders[i]->object->GetComponent<BoxCollider>());
			return true;
		}
	}
	return false;
}

