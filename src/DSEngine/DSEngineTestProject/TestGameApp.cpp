#include <iostream>
#include <stack>

#include <assimp/scene.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/range/adaptor/reversed.hpp>

#include "PBRMaterial.hpp"

#include "TestGameApp.h"
#include "PressSpaceToPlayAudio.h"
#include "MoveParentObject.h"
#include "CameraController.h"

TestGameApp::~TestGameApp()
{
}

void TestGameApp::Init()
{
	// Stub logic
	LOG_TRACE << "TestGameApp Init";

	// Load Shaders


	// Set Camera
	CurrentActiveScene()->mainCamera->UpdateProjectionMatrix(float(width), float(height), 3.1415926f / 4.0f);
	CurrentActiveScene()->mainCamera->SetSkybox(device, context, L"Assets/Skybox/1/Environment1HiDef.cubemap.dds", L"Assets/Skybox/1/Environment1Light.cubemap.dds");
	//CurrentActiveScene()->mainCamera->SetSkybox(device, context, L"Assets/Skybox/mp_cupertin/mp_cupertin.dds", L"Assets/Skybox/mp_cupertin/mp_cupertin_irr.dds");
	CameraController * cameraController = CurrentActiveScene()->mainCamera->AddComponent<CameraController>();

	// Add a light
	LightData light = DirectionalLight(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), 0.8f, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));;
	CurrentActiveScene()->AddLight(light);
	//LightData anotherLight = DirectionalLight(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), 0.8f, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));;
	//CurrentActiveScene()->AddLight(anotherLight);

	


	// Add parent object
	//Object * parentObj = CurrentActiveScene()->LoadModelFile("Assets/Models/Fennekin/a653.dae");
	//Object * parentObj = CurrentActiveScene()->LoadModelFile("Assets/Models/025_Pikachu/0.obj");
	//Object* parentObj = CurrentActiveScene()->LoadModelFile("Assets/Models/Rock/sphereNoNormal.obj");
	Object * parentObj = CurrentActiveScene()->LoadModelFile("Assets/Models/Rock/cube.obj");
	//parentObj->transform->SetLocalScale(0.05f, 0.05f, 0.05f);
	parentObj->transform->SetLocalTranslation(-3.0f, -1.0f, 5.0f);
	//parentObj->transform->SetLocalScale(1.0f, 1.0f, 1.0f);
	
	//for the test
	Collider* collider1 = parentObj->AddComponent<Collider>();
	collider1->SetColliderTransform(0.5f, DirectX::XMQuaternionIdentity(), DirectX::XMVectorSet(-3.0f, -1.0f, 5.0f, 0.0f));
	RigidBody* rigidBody1 = parentObj->AddComponent<RigidBody>();
	rigidBody1->SetPosition(-3.0f, -1.0f, 5.0f);
	//rigidBody1->SetVelocity(1.0f, -1.0f, 0.0f);
	rigidBody1->AddForce(0.0f, -1.0f, 0.0f);
	rigidBody1->SetMass(1.0f);
	rigidBody1->SetWorldMatrix();
	
	

	// Add Components
	PressSpaceToPlayAudio * playAudioComponent = parentObj->AddComponent<PressSpaceToPlayAudio>();
	MoveParentObject * moveParentComponent = parentObj->AddComponent<MoveParentObject>();


	// Add another object
	Object* anotherObject = CurrentActiveScene()->LoadModelFile("Assets/Models/Rock/cube.obj");
	//Object * anotherObject = CurrentActiveScene()->LoadModelFile("Assets/Models/255_Torchic/0.obj");
	//anotherObject->transform->SetLocalScale(0.05f, 0.05f, 0.05f);
	anotherObject->transform->SetLocalTranslation(3.0f, 3.0f, 5.0f);
	anotherObject->transform->SetLocalRotation(0.1f, 0.1f, 0.1f, 1.0f);
	//anotherObject->transform->SetLocalScale(2.0f, 2.0f, 2.0f);

	//for test
	Collider* collider2 = anotherObject->AddComponent<Collider>();
	collider2->SetColliderTransform(0.5f, DirectX::XMQuaternionIdentity(), DirectX::XMVectorSet(3.0f, 3.0f, 5.0f, 0.0f));
	RigidBody* rigidBody2 = anotherObject->AddComponent<RigidBody>();
	rigidBody2->SetPosition(3.0f, 3.0f, 5.0f);
	//rigidBody2->SetVelocity(-1.0f, -1.0f, 0.0f);
	rigidBody2->AddForce(-1.0f, 3.0f, 0.0f);
	rigidBody2->SetMass(1.0f);
	rigidBody2->SetWorldMatrix();
	
	//// Add a ground
	//Object * ground = CurrentActiveScene()->LoadModelFile("Assets/Models/Rock/quad.obj");
	//ground->name = "Ground";
	////ground->transform->SetLocalScale(DirectX::XMVectorSet(2.0f, 2.0f, 2.0f, 0.0f));
	//ground->transform->SetLocalTranslation(DirectX::XMVectorSet(0.0f, -0.01f, 5.0f, 0.0f));
	//const DirectX::XMVECTOR rq = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f,0.0f, 1.0f, 0.0f), DirectX::XM_PIDIV2);
	//ground->transform->SetLocalRotation(rq);

	//Object* groundModelObject = (*ground->transform->GetChildren().begin())->object;
	//MeshRenderer* groundMeshRenderer = groundModelObject->GetComponent<MeshRenderer>();
	//PBRMaterial* groundMaterial = static_cast<PBRMaterial*>(groundMeshRenderer->GetMaterial());
	//groundMaterial->parameters.metalness = 0.0f;
	//groundMaterial->parameters.roughness = 1.0f;
	//groundMaterial->parameters.albedo = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);

	LOG_INFO << "Scene Structure:";
	std::list<Object*> allObjects = CurrentActiveScene()->GetAllObjects();

	std::stack<Object*> objectStack;
	std::stack<int> objectLevel;

	for (Object* obj : boost::adaptors::reverse(allObjects))
	{
		if (obj->transform->GetParent() == nullptr)
		{
			objectStack.push(obj);
			objectLevel.push(0);
		}
	}

	while (!objectStack.empty())
	{
		// Get the object
		Object* currentObject = objectStack.top();
		int currentLevel = objectLevel.top();
		// Process DFS
		objectStack.pop();
		objectLevel.pop();
		std::list<Transform*> children = currentObject->transform->GetChildren();
		for (Transform* child : boost::adaptors::reverse(children))
		{
			objectStack.push(child->object);
			objectLevel.push(currentLevel + 1);
		}

		// Output Info
		std::string tabs = "\t";
		for (int i = 0; i < currentLevel; ++i)
			tabs += "\t";
		LOG_INFO << tabs << currentObject->name;// << "\t{" << to_string(currentObject->GetInstanceID()) << "}";
	}


}
