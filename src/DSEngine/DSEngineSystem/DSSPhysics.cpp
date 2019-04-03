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
	DirectX::XMFLOAT3 direction = ray.GetTarget();
	for (int i = 0; i < boxColliders.size(); i++) {
		DirectX::XMFLOAT3 center = boxColliders[i]->GetCollider()->Center;
		float distanceToSide = sqrt(pow(boxColliders[i]->GetCollider()->Extents.x, 2.0f) + pow(boxColliders[i]->GetCollider()->Extents.y, 2.0f) + pow(boxColliders[i]->GetCollider()->Extents.z, 2.0f));
		float originToDiretion = sqrt(pow((origin.x - direction.x), 2.0) + pow((origin.y - direction.y), 2.0) + pow((origin.z - direction.z), 2.0));
		float originToCenter = sqrt(pow((origin.x - center.x), 2.0) + pow((origin.y - center.y), 2.0) + pow((origin.z - center.z), 2.0));
		float DirectionToCenter = sqrt(pow((direction.x - center.x), 2.0) + pow((direction.y - center.y), 2.0) + pow((direction.z - center.z), 2.0));
		float cos_A = (pow(originToCenter, 2.0) + pow(originToDiretion, 2.0) - pow(DirectionToCenter, 2.0)) / (2 * originToCenter*originToDiretion);
		float sin_A = sqrt(1 - pow(cos_A, 2.0));
		float distanceTocenter = originToCenter * sin_A;
		if (distanceTocenter <= distanceToSide) {
			mHit.SetCollider(boxColliders[i]->object->GetComponent<BoxCollider>());
			return true;
		}
	}
	return false;
}

