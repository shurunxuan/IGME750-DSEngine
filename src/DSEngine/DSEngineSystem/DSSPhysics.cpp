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
	for (int i = 0; i < boxColliders.size(); i++) {
		if (origin.x < boxColliders[i]->GetCollider()->Center.x + boxColliders[i]->GetCollider()->Extents.x
			&& origin.x > boxColliders[i]->GetCollider()->Center.x - boxColliders[i]->GetCollider()->Extents.x
			&& origin.y > boxColliders[i]->GetCollider()->Center.y - boxColliders[i]->GetCollider()->Extents.y
			&& origin.y < boxColliders[i]->GetCollider()->Center.y + boxColliders[i]->GetCollider()->Extents.y)
		{
			mHit.SetCollider(boxColliders[i]->object->GetComponent<Collider>());
			return true;
		}
	}
	return false;
}
