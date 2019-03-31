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
	colliders.clear();
	rigidBodies.clear();
	boxColliders.clear();

	for (Object* object : App->CurrentActiveScene()->allObjects)
	{
		if (object->GetComponent<RigidBody>()) rigidBodies.push_back(object->GetComponent<RigidBody>());
		if (object->GetComponent<Collider>()) colliders.push_back(object->GetComponent<Collider>());
		if (object->GetComponent<BoxCollider>()) boxColliders.push_back(object->GetComponent<BoxCollider>());
	}


	for (int i = 0; i < colliders.size(); i++) {
		if (colliders[i]->GetCollider()->Center.y <= 0.49f) {
			rigidBodies[i]->SetVelocity(rigidBodies[i]->GetVelocity().x, 0, 0);
			rigidBodies[i]->SetInertia(1.0f);
			rigidBodies[i]->AddForce(-rigidBodies[i]->GetDirection().x * 1.0f, 0.0f, 0.0f);
			rigidBodies[i]->AddForce(0.0f, 2.0f, 0.0f);
		}
		else rigidBodies[i]->SetInertia(0.0f);
		for (int j = i + 1; j < colliders.size(); j++) {
			if (colliders[i]->GetCollider()->Intersects(*colliders[j]->GetCollider())) {

				DirectX::XMFLOAT3 temp = rigidBodies[j]->GetVelocity();
				rigidBodies[j]->SetVelocity(rigidBodies[i]->GetVelocity());
				rigidBodies[i]->SetVelocity(temp);
			}
		}
	}


	for (int i = 0; i < rigidBodies.size(); i++) {
		rigidBodies[i]->CalculateWorldMatrix(deltaTime, totalTime);
		BoundingSphere newCollider = BoundingSphere();
		colliders[i]->GetCollider()->Transform(newCollider, 1.0f, DirectX::XMLoadFloat3(&(rigidBodies[i]->GetColliderRotation())), DirectX::XMLoadFloat3(&(rigidBodies[i]->GetPosition())));
		*(colliders[i]->GetCollider()) = newCollider;
		rigidBodies[i]->object->transform->SetLocalTranslation(DirectX::XMLoadFloat3(&(rigidBodies[i]->GetPosition())));
		rigidBodies[i]->object->transform->SetLocalRotation(rigidBodies[i]->GetRotationQuaterinion());
		//rigidBodies[i]->object->transform->SetLocalRotation(sin(totalTime),0.0f,0.0f,0.5f);

		rigidBodies[i]->SetForce(0.0f, -2.0f, 0.0f);
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
	DirectX::XMFLOAT3 direction = ray.GetDirection();
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

//DirectX::XMFLOAT3 origin = ray.GetOrigin();
//for (int i = 0; i < boxColliders.size(); i++) {
//	if (origin.x < boxColliders[i]->GetCollider()->Center.x + boxColliders[i]->GetCollider()->Extents.x
//		&& origin.x > boxColliders[i]->GetCollider()->Center.x - boxColliders[i]->GetCollider()->Extents.x
//		&& origin.y > boxColliders[i]->GetCollider()->Center.y - boxColliders[i]->GetCollider()->Extents.y
//		&& origin.y < boxColliders[i]->GetCollider()->Center.y + boxColliders[i]->GetCollider()->Extents.y
//		&& origin.z < boxColliders[i]->GetCollider()->Center.z + boxColliders[i]->GetCollider()->Extents.z
//		&& origin.z > boxColliders[i]->GetCollider()->Center.z - boxColliders[i]->GetCollider()->Extents.z)
//	{
//		mHit.SetCollider(boxColliders[i]->object->GetComponent<BoxCollider>());
//		return true;
//	}
//}
//return false;