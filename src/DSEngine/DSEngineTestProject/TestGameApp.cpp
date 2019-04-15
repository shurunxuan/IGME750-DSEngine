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
#include "WheelController.h"

TestGameApp::~TestGameApp()
{
}

void TestGameApp::Init()
{
	LOG_TRACE << "TestGameApp Init";

	// Register Inputs
	SInput->RegisterInput("Horizontal", "d", "a", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("Horizontal", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Axis, JoystickLX, -1);
	SInput->RegisterInput("Vertical", "w", "s", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("Vertical", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Axis, JoystickLY, -1);
	SInput->RegisterInput("CameraHorizontal", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Axis, JoystickRX, -1);
	SInput->RegisterInput("CameraHorizontal", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Movement, MouseX, -1);
	SInput->RegisterInput("CameraVertical", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Axis, JoystickRY, -1);
	SInput->RegisterInput("CameraVertical", "", "", "", "", 10.0f, 0.1f, 10.0f, true, Movement, MouseY, -1);
	SInput->RegisterInput("ArrowHorizontal", "joystick right", "joystick left", "right", "left", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("ArrowVertical", "joystick up", "joystick down", "up", "down", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("PlayAudio", "space", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("MoveHorizontal", "l", "j", "joystick b", "joystick x", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("MoveVertical", "i", "k", "joystick y", "joystick a", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);

	// Set Camera
	CurrentActiveScene()->mainCamera->UpdateProjectionMatrix(float(width), float(height), DirectX::XM_PIDIV4);
	CurrentActiveScene()->mainCamera->SetSkybox(device, context, L"Assets/Skybox/1/Environment1HiDef.cubemap.dds", L"Assets/Skybox/1/Environment1Light.cubemap.dds");
	CurrentActiveScene()->mainCamera->transform->SetLocalTranslation(0.0f, 4.0f, -8.0f);
	CameraController * cameraController = CurrentActiveScene()->mainCamera->AddComponent<CameraController>();

	// Add a light
	LightData light = DirectionalLight(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), 0.8f, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));;
	CurrentActiveScene()->AddLight(light);

	


	//-----------------Car Object Structure---------------------
	//  Parent Object     Car(Empty Object) ------> Add RigidBody Component
	//                      /      \
	//                     /        \
	//                    /          \
	//  Children       Wheels        Frame
	//                  /              \
	//     (Add WheelController)      (No Componnent)


	Object * car = CurrentActiveScene()->AddObject("Car");
	car->transform->SetLocalTranslation(0.0f, 0.0f, 1.8f);
	RigidBody * rigidbody = car->AddComponent<RigidBody>();
	rigidbody->SetPosition(0.0f, 0.0f, 4.75f);

	Object * frame = CurrentActiveScene()->LoadModelFile("Assets/Models/cgtrader/car.obj");
	frame->transform->SetLocalTranslation(0.0f, 0.0f, 4.75f);
	frame->transform->SetParent(car->transform);
	
	Object * wheel_1 = CurrentActiveScene()->LoadModelFile("Assets/Models/18-tire/tire_1.obj");
	wheel_1->transform->SetLocalTranslation(-1.4f, 1.0f, 7.0f);
	wheel_1->transform->SetLocalScale(0.05f, 0.05f, 0.05f);
	WheelCollider* wheelCollider_1 = wheel_1->AddComponent<WheelCollider>();
	wheelCollider_1->SetRadius(1.0f);
	wheelCollider_1->SetMaxSteeringAngle(25.0f);
	wheelCollider_1->SetWheelDistance(5.2f);
	wheel_1->transform->SetParent(car->transform);

	Object * wheel_2 = CurrentActiveScene()->LoadModelFile("Assets/Models/18-tire/tire_1.obj");
	wheel_2->transform->SetLocalTranslation(1.4f, 1.0f, 7.0f);
	wheel_2->transform->SetLocalScale(0.05f, 0.05f, 0.05f);
	WheelCollider* wheelCollider_2 = wheel_2->AddComponent<WheelCollider>();
	wheelCollider_2->SetRadius(1.0f);
	wheelCollider_2->SetMaxSteeringAngle(25.0f);
	wheelCollider_2->SetWheelDistance(5.2f);
	wheel_2->transform->SetParent(car->transform);

	Object * wheel_3 = CurrentActiveScene()->LoadModelFile("Assets/Models/18-tire/tire_1.obj");
	wheel_3->transform->SetLocalTranslation(-1.4f, 1.0f, 1.8f);
	wheel_3->transform->SetLocalScale(0.05f, 0.05f, 0.05f);
	WheelCollider* wheelCollider_3 = wheel_3->AddComponent<WheelCollider>();
	wheelCollider_3->SetRadius(1.0f);
	wheelCollider_3->SetMaxSteeringAngle(0.0f);
	wheelCollider_3->SetWheelDistance(5.2f);
	wheel_3->transform->SetParent(car->transform);

	Object * wheel_4 = CurrentActiveScene()->LoadModelFile("Assets/Models/18-tire/tire_1.obj");
	wheel_4->transform->SetLocalTranslation(1.4f, 1.0f, 1.8f);
	wheel_4->transform->SetLocalScale(0.05f, 0.05f, 0.05f);
	WheelCollider* wheelCollider_4 = wheel_4->AddComponent<WheelCollider>();
	wheelCollider_4->SetRadius(1.0f);
	wheelCollider_4->SetMaxSteeringAngle(0.0f);
	wheelCollider_4->SetWheelDistance(5.2f);
	wheel_4->transform->SetParent(car->transform);
	
	// Set Camera parent
	CurrentActiveScene()->mainCamera->transform->SetParent(car->transform);


	// Add Components
	WheelController * wheelController_1 = wheel_1->AddComponent<WheelController>();
	WheelController * wheelController_2 = wheel_2->AddComponent<WheelController>();
	WheelController * wheelController_3 = wheel_3->AddComponent<WheelController>();
	WheelController * wheelController_4 = wheel_4->AddComponent<WheelController>();


	// Add a ground
	Object * ground = CurrentActiveScene()->LoadModelFile("Assets/Models/Rock/quad.obj");
	ground->name = "Ground";
	//ground->transform->SetLocalScale(DirectX::XMVectorSet(2.0f, 2.0f, 2.0f, 0.0f));
	ground->transform->SetLocalTranslation(DirectX::XMVectorSet(0.0f, -0.01f, 5.0f, 0.0f));
	const DirectX::XMVECTOR rq = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), DirectX::XM_PIDIV2);
	ground->transform->SetLocalRotation(rq);
	ground->transform->SetLocalScale(5.0f,5.0f,5.0f);

	Object * groundModelObject = (*ground->transform->GetChildren().begin())->object;
	MeshRenderer * groundMeshRenderer = groundModelObject->GetComponent<MeshRenderer>();
	PBRMaterial * groundMaterial = static_cast<PBRMaterial*>(groundMeshRenderer->GetMaterial());
	groundMaterial->parameters.metalness = 0.0f;
	groundMaterial->parameters.roughness = 1.0f;
	groundMaterial->parameters.albedo = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);


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
