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
#include "WheelController.h"
#include "EngineAudioManager.h"

TestGameApp::~TestGameApp()
{
	delete ppDarkCornerPS;
	delete ppGaussianBlurUPS;
	delete ppGaussianBlurVPS;
	delete ppAddPS;
	delete darkCornerMaterial;
	delete blurUMaterial;
	delete blurVMaterial;
	delete applyBloomMaterial;
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
	blurUMaterial = new PPGaussianBlurMaterial(1, { 1 }, 1, { 2 }, SRendering->GetDefaultPostProcessingVertexShader(), ppGaussianBlurUPS, device);
	blurVMaterial = new PPGaussianBlurMaterial(1, { 2 }, 1, { 1 }, SRendering->GetDefaultPostProcessingVertexShader(), ppGaussianBlurVPS, device);
	blurUMaterial->SetScreenSizePtr(&width, &height);
	blurVMaterial->SetScreenSizePtr(&width, &height);
	SRendering->RegisterPostProcessing(blurUMaterial);
	SRendering->RegisterPostProcessing(blurVMaterial);	
	SRendering->RegisterPostProcessing(blurUMaterial);
	SRendering->RegisterPostProcessing(blurVMaterial);

	ppAddPS = new SimplePixelShader(device, context);
	ppAddPS->LoadShaderFile(L"PPAddPS.cso");
	applyBloomMaterial = new PostProcessingMaterial(2, { 0, 1 }, 1, { 2 }, SRendering->GetDefaultPostProcessingVertexShader(), ppAddPS, device);
	SRendering->RegisterPostProcessing(applyBloomMaterial);

	ppDarkCornerPS = new SimplePixelShader(device, context);
	ppDarkCornerPS->LoadShaderFile(L"PPDarkCornerPS.cso");
	darkCornerMaterial = new PPDarkCornerMaterial(1, { 2 }, 1, { 0 }, SRendering->GetDefaultPostProcessingVertexShader(), ppDarkCornerPS, device);
	darkCornerMaterial->parameters.intensity = 1.0f;
	SRendering->RegisterPostProcessing(darkCornerMaterial);

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
	rigidbody->SetMass(50.0f);

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

	
	// Add parent object
	//Object * parentObj = CurrentActiveScene()->LoadModelFile("Assets/Models/Fennekin/a653.dae");
	//parentObj->name = "Fennekin";
	//parentObj->transform->SetLocalScale(0.05f, 0.05f, 0.05f);
	//parentObj->transform->SetLocalTranslation(-1.0f, 0.0f, 5.0f);

	//auto rotation = parentObj->transform->GetLocalRotation();
	//rotation = DirectX::XMQuaternionMultiply(rotation, DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), DirectX::XM_PIDIV2));
	//parentObj->transform->SetLocalRotation(rotation);
	Object* parentObj = nullptr;
	for (int i = 0; i < 5; ++i)
	{
		Object* obj = CurrentActiveScene()->LoadModelFile("Assets/Models/Rock/sphere.obj");
		obj->transform->SetLocalTranslation((i - 2) * 2.0f, 1.0f, 5.0f);
		MeshRenderer* renderer = obj->transform->GetChildAt(0)->object->GetComponent<MeshRenderer>();
		PBRMaterial* material = static_cast<PBRMaterial*>(renderer->GetMaterial());
		material->transparent = true;
		D3D11_RENDER_TARGET_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
		blendDesc.BlendEnable = TRUE;
		blendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		material->SetBlendMode(blendDesc);
		material->parameters.transparency = 0.8f;
		if (i == 0) parentObj = obj;
	}


	// Add Components
	PressSpaceToPlayAudio * playAudioComponent = parentObj->AddComponent<PressSpaceToPlayAudio>();
	MoveParentObject * moveParentComponent = parentObj->AddComponent<MoveParentObject>();
	WheelController * wheelController_1 = wheel_1->AddComponent<WheelController>();
	WheelController * wheelController_2 = wheel_2->AddComponent<WheelController>();
	WheelController * wheelController_3 = wheel_3->AddComponent<WheelController>();
	WheelController * wheelController_4 = wheel_4->AddComponent<WheelController>();

	AudioSource * audioSource1 = parentObj->AddComponent<AudioSource>();
	audioSource1->Is3D = true;
	audioSource1->Loop = true;

	//audioSource1->SetDopplerScaler(10.0f);
	//audioSource1->SetCurveDistanceScaler(100.0f);
	AudioSource * audioSource2 = parentObj->AddComponent<AudioSource>();

	playAudioComponent->source1 = audioSource1;
	playAudioComponent->source2 = audioSource2;

	audioSource1->LoadAudioFile("Assets/Audio/idle.wav");
	audioSource2->LoadAudioFile("Assets/test2.flac");
	//Add different Engine sounds
	AudioSource* idleAudio = car->AddComponent<AudioSource>();
	idleAudio->Is3D = true;
	idleAudio->Loop = true;
	idleAudio->LoadAudioFile("Assets/Audio/idle.wav");
	AudioSource* startupAudio = car->AddComponent<AudioSource>();
	startupAudio->Is3D = true;
	startupAudio->Loop = false;
	startupAudio->LoadAudioFile("Assets/Audio/startup.wav");
	AudioSource* highOnAudio = car->AddComponent<AudioSource>();
	highOnAudio->Is3D = true;
	highOnAudio->LoadAudioFile("Assets/Audio/high_on.wav");
	AudioSource* midOnAudio = car->AddComponent<AudioSource>();
	midOnAudio->Is3D = true;
	midOnAudio->LoadAudioFile("Assets/Audio/med_on.wav");
	AudioSource* lowOnAudio = car->AddComponent<AudioSource>();
	lowOnAudio->Is3D = true;
	lowOnAudio->LoadAudioFile("Assets/Audio/low_on.wav");
	AudioSource* maxRPMAudio = car->AddComponent<AudioSource>();
	maxRPMAudio->Is3D = true;
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
	//ground->transform->SetLocalScale(DirectX::XMVectorSet(2.0f, 2.0f, 2.0f, 0.0f));
	ground->transform->SetLocalTranslation(DirectX::XMVectorSet(0.0f, -0.01f, 5.0f, 0.0f));
	const DirectX::XMVECTOR rq = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), DirectX::XM_PIDIV2);
	ground->transform->SetLocalRotation(rq);
	ground->transform->SetLocalScale(10.0f, 10.0f, 10.0f);

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