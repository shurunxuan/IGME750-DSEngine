#include <iostream>

#include <assimp/Importer.hpp> 
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>


#include "TestGameApp.h"
#include "PressSpaceToPlayAudio.h"
#include "UnlitMaterial.h"
#include "MoveParentObject.h"
#include "CameraController.h"
#include "PBRMaterial.h"
#include <queue>

TestGameApp::~TestGameApp()
{
	delete vertexShader;
	delete pbrPixelShader;
}

void TestGameApp::Init()
{
	// Stub logic
	LOG_TRACE << "TestGameApp Init";

	// Load Shaders
	// TODO: Could this be done by a resource manager?
	vertexShader = new SimpleVertexShader(device, context);
	pbrPixelShader = new SimplePixelShader(device, context);

	vertexShader->LoadShaderFile(L"VertexShader.cso");
	pbrPixelShader->LoadShaderFile(L"PBRPixelShader.cso");

	// Set Camera
	CurrentActiveScene()->mainCamera->UpdateProjectionMatrix(float(width), float(height), 3.1415926f / 4.0f);
	CurrentActiveScene()->mainCamera->SetSkybox(device, context, L"Assets/Skybox/1/Environment1HiDef.cubemap.dds", L"Assets/Skybox/1/Environment1Light.cubemap.dds");
	//CurrentActiveScene()->mainCamera->SetSkybox(device, context, L"Assets/Skybox/mp_cupertin/mp_cupertin.dds", L"Assets/Skybox/mp_cupertin/mp_cupertin_irr.dds");
	CameraController * cameraController = CurrentActiveScene()->mainCamera->AddComponent<CameraController>();

	// Add a light
	Light light = DirectionalLight(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), 1.0f, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));;
	CurrentActiveScene()->AddLight(light);

	// Add parent object
	Object * parentObj = CurrentActiveScene()->LoadModelFile("Assets/Models/025_Pikachu/0.obj");
	parentObj->transform->SetLocalScale(0.05f, 0.05f, 0.05f);
	parentObj->transform->SetLocalTranslation(-1.0f, 0.0f, 5.0f);

	std::queue<Transform*> transformQueue;

	transformQueue.push(parentObj->transform);

	while (!transformQueue.empty())
	{
		// Get the transform
		Transform* transform = transformQueue.front();

		// Process BFS
		transformQueue.pop();
		std::list<Transform*> children = transform->GetChildren();
		for (Transform* child : children)
		{
			transformQueue.push(child);
		}

		// Process material
		Object* obj = transform->object;
		std::list<MeshRenderer*> meshRenderers = obj->GetComponents<MeshRenderer>();
		for (MeshRenderer* meshRenderer : meshRenderers)
		{
			// Material
			std::shared_ptr<PBRMaterial> pbrMaterial = std::make_shared<PBRMaterial>(vertexShader, pbrPixelShader, device, context);
			// Yellow
			pbrMaterial->parameters.albedo = { 1.0f, 0.765557f, 0.336057f };
			meshRenderer->SetMaterial(pbrMaterial);
		}
	}


	// Add Components
	PressSpaceToPlayAudio* playAudioComponent = parentObj->AddComponent<PressSpaceToPlayAudio>();
	MoveParentObject* moveParentComponent = parentObj->AddComponent<MoveParentObject>();


	// Add another object
	Object* anotherObject = CurrentActiveScene()->LoadModelFile("Assets/Models/255_Torchic/0.obj");
	anotherObject->transform->SetLocalScale(0.05f, 0.05f, 0.05f);
	anotherObject->transform->SetLocalTranslation(1.0f, 0.0f, 5.0f);

	transformQueue.push(anotherObject->transform);

	while (!transformQueue.empty())
	{
		// Get the transform
		Transform* transform = transformQueue.front();

		// Process BFS
		transformQueue.pop();
		std::list<Transform*> children = transform->GetChildren();
		for (Transform* child : children)
		{
			transformQueue.push(child);
		}

		// Process material
		Object* obj = transform->object;
		std::list<MeshRenderer*> meshRenderers = obj->GetComponents<MeshRenderer>();
		for (MeshRenderer* meshRenderer : meshRenderers)
		{
			// Material
			std::shared_ptr<PBRMaterial> pbrMaterial = std::make_shared<PBRMaterial>(vertexShader, pbrPixelShader, device, context);
			// Yellow
			pbrMaterial->parameters.albedo = { 0.955008f, 0.637427f, 0.538163f };
			meshRenderer->SetMaterial(pbrMaterial);
		}
	}


	std::list<Object*> allObjects = CurrentActiveScene()->GetAllObjects();

	for (Object* obj : allObjects)
	{
		if (obj->transform->GetParent() == nullptr)
			LOG_INFO << "\tObject: " << obj->name << "\t{" << to_string(obj->GetInstanceID()) << "}";
		else
			LOG_INFO << "\tObject: " << obj->name << "\t{" << to_string(obj->GetInstanceID()) << "}\tParent: "
			<< obj->transform->GetParent()->object->name << "\t{" << to_string(obj->transform->GetParent()->object->GetInstanceID()) << "}";
	}
}
