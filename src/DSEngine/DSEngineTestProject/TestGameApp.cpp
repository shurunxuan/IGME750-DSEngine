#include <iostream>
#include <stack>

#include <assimp/scene.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/range/adaptor/reversed.hpp>

#include "PBRMaterial.hpp"
#include "AudioSource.hpp"

#include "TestGameApp.h"
#include "PressSpaceToPlayAudio.h"
#include "MoveParentObject.h"
#include "CameraController.h"
#include "SSAOMaterial.h"
#include "WheelController.h"
#include "EngineAudioManager.h"

TestGameApp::~TestGameApp()
{
	delete ppDarkCornerPS;
	delete ppGaussianBlurUPS;
	delete ppGaussianBlurVPS;
	delete ppAddPS;
	delete ppMultiplyPS;
	delete ppSSAOPS;
	delete ppSSAOVS;
	delete darkCornerMaterial;
	delete blurUMaterial;
	delete blurVMaterial;
	delete blurSSAOUMaterial;
	delete blurSSAOVMaterial;
	delete applyBloomMaterial;
	delete applySSAOMaterial;
	delete ssaoMaterial;
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
	SInput->RegisterInput("CameraHorizontal", "h", "f", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("CameraVertical", "t", "g", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("ArrowHorizontal", "joystick right", "joystick left", "right", "left", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("ArrowVertical", "joystick up", "joystick down", "up", "down", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("PlayAudio1", "z", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("PauseAudio1", "x", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("StopAudio1", "c", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("PlayAudio2", "v", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("PauseAudio2", "b", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("StopAudio2", "n", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("MoveHorizontal", "l", "j", "joystick b", "joystick x", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("MoveVertical", "i", "k", "joystick y", "joystick a", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("Audio1PitchUp", "1", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("Audio1PitchDown", "2", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("Audio1VolumeUp", "3", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("Audio1VolumeDown", "4", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("Brake", "space", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);

	// Register post processing effects
	ppGaussianBlurUPS = new SimplePixelShader(device, context);
	ppGaussianBlurVPS = new SimplePixelShader(device, context);
	ppGaussianBlurUPS->LoadShaderFile(L"PPGaussianBlurUPS.cso");
	ppGaussianBlurVPS->LoadShaderFile(L"PPGaussianBlurVPS.cso");
	blurUMaterial = new PPGaussianBlurMaterial(1, { 1 }, 1, { 4 }, SRendering->GetDefaultPostProcessingVertexShader(), ppGaussianBlurUPS, device);
	blurVMaterial = new PPGaussianBlurMaterial(1, { 4 }, 1, { 5 }, SRendering->GetDefaultPostProcessingVertexShader(), ppGaussianBlurVPS, device);
	blurUMaterial->SetScreenSizePtr(&width, &height);
	blurVMaterial->SetScreenSizePtr(&width, &height);
	SRendering->RegisterPostProcessing(blurUMaterial); // 1 -> 4
	SRendering->RegisterPostProcessing(blurVMaterial); // 4 -> 5	

	ppAddPS = new SimplePixelShader(device, context);
	ppAddPS->LoadShaderFile(L"PPAddPS.cso");
	applyBloomMaterial = new PostProcessingMaterial(2, { 0, 5 }, 1, { 6 }, SRendering->GetDefaultPostProcessingVertexShader(), ppAddPS, device);
	SRendering->RegisterPostProcessing(applyBloomMaterial); // 0 + 5 -> 6

	ppSSAOPS = new SimplePixelShader(device, context);
	ppSSAOPS->LoadShaderFile(L"PPSSAOPS.cso");
	ppSSAOVS = new SimpleVertexShader(device, context);
	ppSSAOVS->LoadShaderFile(L"PPSSAOVS.cso");
	ssaoMaterial = new SSAOMaterial(2, { 3, -1 }, 1, { 7 }, ppSSAOVS, ppSSAOPS, device);
	ssaoMaterial->SetCamera(CurrentActiveScene()->mainCamera);
	SRendering->RegisterPostProcessing(ssaoMaterial); // -1 & 3 -> 7

	blurSSAOUMaterial = new PPGaussianBlurMaterial(1, { 7 }, 1, { 4 }, SRendering->GetDefaultPostProcessingVertexShader(), ppGaussianBlurUPS, device);
	blurSSAOVMaterial = new PPGaussianBlurMaterial(1, { 4 }, 1, { 5 }, SRendering->GetDefaultPostProcessingVertexShader(), ppGaussianBlurVPS, device);
	blurSSAOUMaterial->SetScreenSizePtr(&width, &height);
	blurSSAOVMaterial->SetScreenSizePtr(&width, &height);
	SRendering->RegisterPostProcessing(blurSSAOUMaterial); // 7 -> 4
	SRendering->RegisterPostProcessing(blurSSAOVMaterial); // 4 -> 5

	ppMultiplyPS = new SimplePixelShader(device, context);
	ppMultiplyPS->LoadShaderFile(L"PPMultiplyPS.cso");
	applySSAOMaterial = new PostProcessingMaterial(2, { 6, 5 }, 1, { 7 }, SRendering->GetDefaultPostProcessingVertexShader(), ppMultiplyPS, device);
	SRendering->RegisterPostProcessing(applySSAOMaterial); // 6 * 5 -> 7

	ppDarkCornerPS = new SimplePixelShader(device, context);
	ppDarkCornerPS->LoadShaderFile(L"PPDarkCornerPS.cso");
	darkCornerMaterial = new PPDarkCornerMaterial(1, { 7 }, 1, { 0 }, SRendering->GetDefaultPostProcessingVertexShader(), ppDarkCornerPS, device);
	darkCornerMaterial->parameters.intensity = 1.0f;
	SRendering->RegisterPostProcessing(darkCornerMaterial); // 7 -> 0
	// 0 -> screen

	// Set Camera
	CurrentActiveScene()->mainCamera->UpdateProjectionMatrix(float(width), float(height), DirectX::XM_PIDIV4);
	//CurrentActiveScene()->mainCamera->SetSkybox(device, context, L"Assets/Skybox/mp_cupertin/mp_cupertin.dds", L"Assets/Skybox/mp_cupertin/mp_cupertin_irr.dds");
	CurrentActiveScene()->mainCamera->SetSkybox(device, context, L"Assets/Skybox/1/Environment1HiDef.cubemap.dds", L"Assets/Skybox/1/Environment1Light.cubemap.dds");
	CurrentActiveScene()->mainCamera->transform->SetLocalTranslation(0.0f, 2.0f, -6.25f);
	//CurrentActiveScene()->mainCamera->AddComponent<CameraController>();
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

	Object* car = CurrentActiveScene()->AddObject("AventHolder");
	Object* avent = CurrentActiveScene()->LoadModelFile("Assets/Models/Aventador/Avent.obj");
	avent->transform->SetParent(car->transform);
	avent->transform->SetLocalRotation(DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), DirectX::XM_PIDIV2));

	//Object * car = CurrentActiveScene()->AddObject("Car");
	//car->transform->SetLocalTranslation(0.0f, 0.0f, 0.0f);
	RigidBody * rigidbody = car->AddComponent<RigidBody>();
	rigidbody->SetPosition(0.0f, 0.0f, 0.0f);
	rigidbody->SetMass(2000.0f);

	//Object * frame = CurrentActiveScene()->LoadModelFile("Assets/Models/cgtrader/car.obj");
	//frame->transform->SetLocalTranslation(0.0f, 0.0f, 0.0f);
	//frame->transform->SetParent(car->transform);


	Object* wheel_1 = CurrentActiveScene()->AddObject("wheel_1");


	Object* wheel_1_mesh = CurrentActiveScene()->FindObjectByName("Mesh212_032wheel3_032Group1_032Lamborghini_Aventador1_032Model");
	wheel_1->transform->SetLocalTranslation(-0.9698001f, 0.431005f, 1.554001f);
	wheel_1->transform->SetParent(car->transform);
	for (int i = 208; i <= 210; ++i)
	{
		std::string name = "Mesh" + std::to_string(i) + "_032Group12_032wheel3_032Group1_032Lamborghini_Aventador1_032Model";
		std::list<Object*> objects = CurrentActiveScene()->FindObjectsByName(name);
		for (Object* object : objects)
			object->transform->SetParent(wheel_1->transform);
	}
	for (int i = 211; i <= 223; ++i)
	{
		std::string name = "Mesh" + std::to_string(i) + "_032wheel3_032Group1_032Lamborghini_Aventador1_032Model";
		std::list<Object*> objects = CurrentActiveScene()->FindObjectsByName(name);
		for (Object* object : objects)
			object->transform->SetParent(wheel_1->transform);
	}

	Object* wheel_2 = CurrentActiveScene()->AddObject("wheel_2");
	wheel_2->transform->SetLocalTranslation(0.9698001f, 0.431005f, 1.554001f);
	wheel_2->transform->SetParent(car->transform);
	for (int i = 161; i <= 163; ++i)
	{
		std::string name = "Mesh" + std::to_string(i) + "_032Group7_032wheel2_032Group1_032Lamborghini_Aventador1_032Model";
		std::list<Object*> objects = CurrentActiveScene()->FindObjectsByName(name);
		for (Object* object : objects)
			object->transform->SetParent(wheel_2->transform);
	}
	for (int i = 164; i <= 176; ++i)
	{
		std::string name = "Mesh" + std::to_string(i) + "_032wheel2_032Group1_032Lamborghini_Aventador1_032Model";
		std::list<Object*> objects = CurrentActiveScene()->FindObjectsByName(name);
		for (Object* object : objects)
			object->transform->SetParent(wheel_2->transform);
	}

	Object* wheel_3 = CurrentActiveScene()->AddObject("wheel_3");
	wheel_3->transform->SetLocalTranslation(-0.9698001f, 0.431005f, -1.528f);
	wheel_3->transform->SetParent(car->transform);
	for (int i = 224; i <= 226; ++i)
	{
		std::string name = "Mesh" + std::to_string(i) + "_032Group13_032wheel4_032Group1_032Lamborghini_Aventador1_032Model";
		std::list<Object*> objects = CurrentActiveScene()->FindObjectsByName(name);
		for (Object* object : objects)
			object->transform->SetParent(wheel_3->transform);
	}
	for (int i = 227; i <= 239; ++i)
	{
		std::string name = "Mesh" + std::to_string(i) + "_032wheel4_032Group1_032Lamborghini_Aventador1_032Model";
		std::list<Object*> objects = CurrentActiveScene()->FindObjectsByName(name);
		for (Object* object : objects)
			object->transform->SetParent(wheel_3->transform);
	}

	Object* wheel_4 = CurrentActiveScene()->AddObject("wheel_4");
	wheel_4->transform->SetLocalTranslation(0.9698001f, 0.431005f, -1.528f);
	wheel_4->transform->SetParent(car->transform);
	for (int i = 57; i <= 59; ++i)
	{
		std::string name = "Mesh" + std::to_string(i) + "_032Group5_032wheel1_032Group1_032Lamborghini_Aventador1_032Model";
		std::list<Object*> objects = CurrentActiveScene()->FindObjectsByName(name);
		for (Object* object : objects)
			object->transform->SetParent(wheel_4->transform);
	}
	for (int i = 60; i <= 72; ++i)
	{
		std::string name = "Mesh" + std::to_string(i) + "_032wheel1_032Group1_032Lamborghini_Aventador1_032Model";
		std::list<Object*> objects = CurrentActiveScene()->FindObjectsByName(name);
		for (Object* object : objects)
			object->transform->SetParent(wheel_4->transform);
	}

	//Object * wheel_1 = CurrentActiveScene()->LoadModelFile("Assets/Models/18-tire/tire_1.obj");
	//wheel_1->transform->SetLocalTranslation(-1.4f, 1.0f, 2.5f);
	//wheel_1->transform->SetLocalScale(0.05f, 0.05f, 0.05f);
	WheelCollider* wheelCollider_1 = wheel_1->AddComponent<WheelCollider>();
	wheelCollider_1->SetRadius(1.0f);
	wheelCollider_1->SetMaxSteeringAngle(25.0f);
	wheelCollider_1->SetWheelDistance(5.2f);
	//wheel_1->transform->SetParent(car->transform);

	//Object * wheel_2 = CurrentActiveScene()->LoadModelFile("Assets/Models/18-tire/tire_1.obj");
	//wheel_2->transform->SetLocalTranslation(1.4f, 1.0f, 2.5f);
	//wheel_2->transform->SetLocalScale(0.05f, 0.05f, 0.05f);
	WheelCollider* wheelCollider_2 = wheel_2->AddComponent<WheelCollider>();
	wheelCollider_2->SetRadius(1.0f);
	wheelCollider_2->SetMaxSteeringAngle(25.0f);
	wheelCollider_2->SetWheelDistance(5.2f);
	//wheel_2->transform->SetParent(car->transform);

	//Object * wheel_3 = CurrentActiveScene()->LoadModelFile("Assets/Models/18-tire/tire_1.obj");
	//wheel_3->transform->SetLocalTranslation(-1.4f, 1.0f, -2.7f);
	//wheel_3->transform->SetLocalScale(0.05f, 0.05f, 0.05f);
	WheelCollider* wheelCollider_3 = wheel_3->AddComponent<WheelCollider>();
	wheelCollider_3->SetRadius(1.0f);
	wheelCollider_3->SetMaxSteeringAngle(0.0f);
	wheelCollider_3->SetWheelDistance(5.2f);
	//wheel_3->transform->SetParent(car->transform);

	//Object * wheel_4 = CurrentActiveScene()->LoadModelFile("Assets/Models/18-tire/tire_1.obj");
	//wheel_4->transform->SetLocalTranslation(1.4f, 1.0f, -2.7f);
	//wheel_4->transform->SetLocalScale(0.05f, 0.05f, 0.05f);
	WheelCollider* wheelCollider_4 = wheel_4->AddComponent<WheelCollider>();
	wheelCollider_4->SetRadius(1.0f);
	wheelCollider_4->SetMaxSteeringAngle(0.0f);
	wheelCollider_4->SetWheelDistance(5.2f);
	//wheel_4->transform->SetParent(car->transform);

	//Object* cameraPosition = CurrentActiveScene()->AddObject("cameraPosition");
	//cameraPosition->transform->SetLocalTranslation(0.0f, 2.0f, -6.25f);
	//cameraPosition->transform->SetParent(car->transform);

	CameraController* cameraController = CurrentActiveScene()->mainCamera->AddComponent<CameraController>();
	cameraController->following = car->transform;

	// Set Camera parent
	//CurrentActiveScene()->mainCamera->transform->SetParent(car->transform);


	// Add parent object
	//Object * parentObj = CurrentActiveScene()->LoadModelFile("Assets/Models/Fennekin/a653.dae");
	//parentObj->name = "Fennekin";
	//parentObj->transform->SetLocalScale(0.05f, 0.05f, 0.05f);
	//parentObj->transform->SetLocalTranslation(-1.0f, 0.0f, 5.0f);

	//auto rotation = parentObj->transform->GetLocalRotation();
	//rotation = DirectX::XMQuaternionMultiply(rotation, DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), DirectX::XM_PIDIV2));
	//parentObj->transform->SetLocalRotation(rotation);
	//Object* parentObj = nullptr;
	//for (int i = 0; i < 5; ++i)
	//{
	//	Object* obj = CurrentActiveScene()->LoadModelFile("Assets/Models/Rock/sphere.obj");
	//	obj->transform->SetLocalTranslation((i - 2) * 2.0f, 1.0f, 5.0f);
	//	MeshRenderer* renderer = obj->transform->GetChildAt(0)->object->GetComponent<MeshRenderer>();
	//	PBRMaterial* material = static_cast<PBRMaterial*>(renderer->GetMaterial());
	//	material->transparent = true;
	//	D3D11_RENDER_TARGET_BLEND_DESC blendDesc;
	//	ZeroMemory(&blendDesc, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
	//	blendDesc.BlendEnable = TRUE;
	//	blendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	//	blendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//	blendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	//	blendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	//	blendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	//	blendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//	blendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//	material->SetBlendMode(blendDesc);
	//	material->parameters.transparency = 0.8f;
	//	if (i == 0) parentObj = obj;
	//}


	// Add Components
	WheelController * wheelController_1 = wheel_1->AddComponent<WheelController>();
	WheelController * wheelController_2 = wheel_2->AddComponent<WheelController>();
	WheelController * wheelController_3 = wheel_3->AddComponent<WheelController>();
	WheelController * wheelController_4 = wheel_4->AddComponent<WheelController>();

	//AudioSource * audioSource1 = parentObj->AddComponent<AudioSource>();
	//audioSource1->Is3D = true;
	//audioSource1->Loop = true;

	////audioSource1->SetDopplerScaler(10.0f);
	////audioSource1->SetCurveDistanceScaler(100.0f);
	//AudioSource * audioSource2 = parentObj->AddComponent<AudioSource>();

	//playAudioComponent->source1 = audioSource1;
	//playAudioComponent->source2 = audioSource2;

	//audioSource1->LoadAudioFile("Assets/Audio/idle.wav");
	//audioSource2->LoadAudioFile("Assets/test2.flac");
	//Add different Engine sounds
	AudioSource* idleAudio = car->AddComponent<AudioSource>();
	idleAudio->Is3D = true;
	idleAudio->Loop = true;
	idleAudio->LoadAudioFile("Assets/Audio/idle.wav");
	AudioSource* startupAudio = car->AddComponent<AudioSource>();
	startupAudio->Is3D = true;
	startupAudio->LoadAudioFile("Assets/Audio/startup.wav");
	startupAudio->SetVolume(0.6f);
	AudioSource* highOnAudio = car->AddComponent<AudioSource>();
	highOnAudio->Is3D = true;
	highOnAudio->Loop = true;
	highOnAudio->LoadAudioFile("Assets/Audio/high_on.wav");
	AudioSource* midOnAudio = car->AddComponent<AudioSource>();
	midOnAudio->Is3D = true;
	midOnAudio->Loop = true;
	midOnAudio->LoadAudioFile("Assets/Audio/med_on.wav");
	AudioSource* lowOnAudio = car->AddComponent<AudioSource>();
	lowOnAudio->Is3D = true;
	lowOnAudio->Loop = true;
	lowOnAudio->LoadAudioFile("Assets/Audio/low_on.wav");
	AudioSource* maxRPMAudio = car->AddComponent<AudioSource>();
	maxRPMAudio->Is3D = true;
	maxRPMAudio->Loop = true;
	maxRPMAudio->LoadAudioFile("Assets/Audio/maxRPM.wav");
	EngineAudioManager* audioManager = car->AddComponent<EngineAudioManager>();
	audioManager->idle = idleAudio;
	audioManager->startup = startupAudio;
	audioManager->highOn = highOnAudio;
	audioManager->midOn = midOnAudio;
	audioManager->lowOn = lowOnAudio;
	audioManager->maxRPM = maxRPMAudio;

	//Audio Manager:component, and 

	// Add a ground
	Object * ground = CurrentActiveScene()->LoadModelFile("Assets/Models/Rock/quad.obj");
	ground->name = "Ground";
	ground->transform->SetLocalScale(DirectX::XMVectorSet(200.0f, 200.0f, 200.0f, 0.0f));
	ground->transform->SetLocalTranslation(DirectX::XMVectorSet(0.0f, -0.01f, 5.0f, 0.0f));
	const DirectX::XMVECTOR rq = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), DirectX::XM_PIDIV2);
	ground->transform->SetLocalRotation(rq);

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
		LOG_INFO << tabs << currentObject->name << "\t{" << to_string(currentObject->GetInstanceID()) << "}";
	}


}