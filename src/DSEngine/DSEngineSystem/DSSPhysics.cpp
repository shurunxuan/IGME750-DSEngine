#include "DSSPhysics.h"
#include "DSEngineApp.h"

#define Cdrag 0.4257f
#define Crr 12.8f

using namespace DirectX;

DSSPhysics* SPhysics = nullptr;

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
	wheelColliders.clear();

	for (Object* object : App->CurrentActiveScene()->allObjects)
	{
		if (object->GetComponent<RigidBody>()) rigidBodies.push_back(object->GetComponent<RigidBody>());
		if (object->GetComponent<SphereCollider>()) colliders.push_back(object->GetComponent<SphereCollider>());
		if (object->GetComponent<BoxCollider>()) boxColliders.push_back(object->GetComponent<BoxCollider>());
		if (object->GetComponent<WheelCollider>()) wheelColliders.push_back(object->GetComponent<WheelCollider>());
	}

	HandleCollision(deltaTime, totalTime);
	Simulate(deltaTime, totalTime);
	CarSimulate(deltaTime, totalTime);

}

void DSSPhysics::HandleCollision(float deltaTime, float totalTime)
{
	for (int i = 0; i < colliders.size(); i++) {
		if (colliders[i]->object->GetComponent<RigidBody>()) {
			RigidBody* rigidBody = colliders[i]->object->GetComponent<RigidBody>();
			if (colliders[i]->GetCollider()->Center.y <= colliders[i]->GetCollider()->Radius - 0.01f) {
				rigidBody->SetInertia(1.0f);
				rigidBody->SetVelocity(rigidBody->GetVelocity().x, sqrt(0.4f * pow(rigidBody->GetVelocity().y, 2.0f)), rigidBody->GetVelocity().z);
				DirectX::XMFLOAT3 rawAngularVel = rigidBody->GetVelocity();
				float factor = rigidBody->GetInertia() / 3.1415926f * 15.0f;
				rawAngularVel = DirectX::XMFLOAT3(rawAngularVel.z * factor, rawAngularVel.y * factor, rawAngularVel.x * factor);
				rigidBody->SetAngularVelocity(rawAngularVel);
				rigidBody->AddForce(-rigidBody->GetDirection().x * 0.5f, 0.0f, -rigidBody->GetDirection().z * 0.5f);
				rigidBody->AddForce(0.0f, 2.0f, 0.0f);
				rigidBody->SetCollisionPoint(0.0f, -0.01f, 0.0f);
			}
			else {
				rigidBody->SetInertia(0.0f);
			}
		}

		for (int j = i + 1; j < colliders.size(); j++) {
			if (colliders[i]->GetCollider()->Intersects(*colliders[j]->GetCollider())) {
				if (colliders[i]->object->GetComponent<RigidBody>() && colliders[j]->object->GetComponent<RigidBody>()) {
					RigidBody* rigidBody_i = colliders[i]->object->GetComponent<RigidBody>();
					RigidBody* rigidBody_j = colliders[j]->object->GetComponent<RigidBody>();
					DirectX::XMFLOAT3 temp = rigidBody_j->GetVelocity();
					DirectX::XMFLOAT3 tempAngu = rigidBody_j->GetAngularVelocity();
					rigidBody_j->SetVelocity(rigidBody_i->GetVelocity());
					rigidBody_j->SetAngularVelocity(rigidBody_i->GetAngularVelocity());
					rigidBody_i->SetVelocity(temp);
					rigidBody_i->SetAngularVelocity(tempAngu);
				}
				else {
					colliders[i]->object->GetComponent<RigidBody>()->SetVelocity(0.0f, 0.0f, 0.0f);
					colliders[i]->object->GetComponent<RigidBody>()->SetAngularVelocity(0.0f, 0.0f, 0.0f);
					colliders[j]->object->GetComponent<RigidBody>()->SetVelocity(0.0f, 0.0f, 0.0f);
					colliders[j]->object->GetComponent<RigidBody>()->SetAngularVelocity(0.0f, 0.0f, 0.0f);
				}
			}
		}
	}

}

void DSSPhysics::Simulate(float deltaTime, float totalTime)
{
	for (int i = 0; i < rigidBodies.size(); i++) {
		rigidBodies[i]->CalculateWorldMatrix(deltaTime, totalTime);
		
		DirectX::XMVECTOR tempPos = rigidBodies[i]->object->transform->GetLocalTranslation();
		tempPos = DirectX::XMVectorAdd(tempPos, DirectX::XMLoadFloat3(&rigidBodies[i]->GetColliderPosition()));
		rigidBodies[i]->object->transform->SetLocalTranslation(tempPos);

		DirectX::XMVECTOR qx = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rigidBodies[i]->GetColliderRotation().x);
		DirectX::XMVECTOR qy = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rigidBodies[i]->GetColliderRotation().y);
		DirectX::XMVECTOR qz = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rigidBodies[i]->GetColliderRotation().z);
		DirectX::XMVECTOR temp = DirectX::XMQuaternionMultiply(rigidBodies[i]->object->transform->GetLocalRotation(), DirectX::XMQuaternionMultiply(DirectX::XMQuaternionMultiply(qz, qy), qx));
		rigidBodies[i]->object->transform->SetLocalRotation(temp);

		if (rigidBodies[i]->object->GetComponent<SphereCollider>()) {
			rigidBodies[i]->SetForce(0.0f, -2.0f, 0.0f);
		}
	}

	for (int i = 0; i < colliders.size(); i++) {
		if (colliders[i]->object->GetComponent<RigidBody>()) {
			RigidBody* rigidBody = colliders[i]->object->GetComponent<RigidBody>();
			DirectX::BoundingSphere newCollider;
			newCollider.Radius = colliders[i]->GetCollider()->Radius;
			newCollider.Transform(newCollider, 1.0f, DirectX::XMLoadFloat3(&(rigidBody->GetColliderRotation())), DirectX::XMLoadFloat3(&(rigidBody->GetPosition())));
			*(colliders[i]->GetCollider()) = newCollider;
		}
		else {
			DirectX::BoundingSphere newCollider;
			newCollider.Radius = colliders[i]->GetCollider()->Radius;
			newCollider.Transform(newCollider, 1.0f, colliders[i]->object->transform->GetLocalRotation(), colliders[i]->object->transform->GetGlobalTranslation());
			*(colliders[i]->GetCollider()) = newCollider;
		}
	}


}

void DSSPhysics::CarSimulate(float deltaTime, float totalTime)
{
	for (int i = 0; i < wheelColliders.size(); i++) {
		float motorTorque = wheelColliders[i]->GetMotorTorque();
		float brakeTorque = wheelColliders[i]->GetBrakeTorque();
		if (wheelColliders[i]->object->transform->GetParent()) {
			DirectX::XMVECTOR totalTorque;
			DirectX::XMVECTOR dragTorque;
			DirectX::XMVECTOR rrTorque;
			DirectX::XMVECTOR velocity;
			DirectX::XMVECTOR rotation;
			DirectX::XMVECTOR rotationRightAxis;
			DirectX::XMVECTOR accleration;
			DirectX::XMVECTOR right = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
			RigidBody* rigidbody = wheelColliders[i]->object->transform->GetParent()->object->GetComponent<RigidBody>();
			DirectX::XMFLOAT3 vel = rigidbody->GetVelocity();
			float velMagnitude = sqrt((vel.x * vel.x) + (vel.y * vel.y) + (vel.z * vel.z));
			float direction = 1.0f;

			//--------------Tranlation----------------
			totalTorque = (motorTorque + brakeTorque) * (DirectX::XMVector3Normalize(wheelColliders[i]->object->transform->GetParent()->Forward()));
			dragTorque = (-Cdrag) * velMagnitude * DirectX::XMLoadFloat3(&rigidbody->GetVelocity());
			rrTorque = (-Crr) * DirectX::XMLoadFloat3(&rigidbody->GetVelocity());
			totalTorque = totalTorque + dragTorque + rrTorque;
			accleration = totalTorque / wheelColliders[i]->GetMass();
			velocity = DirectX::XMLoadFloat3(&vel);
			velocity += (accleration * deltaTime);
			DirectX::XMStoreFloat3(&vel, velocity);
			rigidbody->SetVelocity(vel);

			if (DirectX::XMVector3NearEqual(DirectX::XMVector3Normalize(wheelColliders[i]->object->transform->GetParent()->Forward()), DirectX::XMVector3Normalize(velocity), DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f))) {
				direction = 1.0f;
			}
			else direction = -1.0f;


			//------------WheelSteeringAngle------------------
			wheelColliders[i]->SetPreviousAngle(wheelColliders[i]->GetCurrentAngle());
			wheelColliders[i]->SetCurrentAngle((wheelColliders[i]->GetSteerFactor() * wheelColliders[i]->GetMaxSteeringAngle()));
			float deltaAngle = wheelColliders[i]->GetCurrentAngle() - wheelColliders[i]->GetPreviousAngle();
			rotation = wheelColliders[i]->object->transform->GetLocalRotation();
			rotationRightAxis = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), deltaAngle / 57.1f );
			rotation = DirectX::XMQuaternionMultiply(rotation, rotationRightAxis);
			wheelColliders[i]->object->transform->SetLocalRotation(rotation);

			//------------WheelRolling------------------------
			right = DirectX::XMVector3Normalize(DirectX::XMVector4Transform(right, XMMatrixTranspose(wheelColliders[i]->object->transform->GetLocalWorldMatrix())));
			rotation = wheelColliders[i]->object->transform->GetLocalRotation();
			rotationRightAxis = DirectX::XMQuaternionRotationAxis(right, direction * deltaTime * velMagnitude / (2.0f * 3.1415926f * wheelColliders[i]->GetRadius()) * 360.0f / 57.3f);
			rotation = DirectX::XMQuaternionMultiply(rotation, rotationRightAxis);
			wheelColliders[i]->object->transform->SetLocalRotation(rotation);

			//------------CarRotation-------------------------
			rotation = rigidbody->object->transform->GetLocalRotation();
			rotationRightAxis = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), direction * deltaTime * velMagnitude * wheelColliders[i]->GetCurrentAngle() / 57.3f / (wheelColliders[i]->GetWheelDistance() * 2.0f) );
			rotation = DirectX::XMQuaternionMultiply(rotation, rotationRightAxis);
			rigidbody->object->transform->SetLocalRotation(rotation);
		}
		else {

		}
		wheelColliders[i]->SetMotorTorque(0.0f);
		wheelColliders[i]->SetBrakeTorque(0.0f);
	}
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
