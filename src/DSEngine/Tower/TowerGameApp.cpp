#include "TowerGameApp.h"
#include "CSVReader.h"
#include "MoveObject.h"
#include "UnlitMaterial.h"
#include "TaskDeck.h"
#include "BrickDeck.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/range/adaptor/reversed.hpp>

DirectX::XMVECTOR baseRotation = DirectX::XMVectorSet(0.000000f, 0.233445f, 0.000000f, 0.972370f);

TowerGameApp::~TowerGameApp()
{
	delete unlitShader;
}

void TowerGameApp::Init()
{
	srand(unsigned(time(0)));
	SInput->RegisterInput("Horizontal", "d", "a", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("Horizontal", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Axis, JoystickLX, -1);
	SInput->RegisterInput("Vertical", "w", "s", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("Vertical", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Axis, JoystickLY, -1);
	SInput->RegisterInput("Test", "mouse 0", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);

	CurrentActiveScene()->mainCamera->UpdateProjectionMatrix(float(width), float(height), DirectX::XM_PI / 3.0f);
	//CurrentActiveScene()->mainCamera->transform->SetLocalTranslation(0.0f, 0.0f, 0.0f);
	CurrentActiveScene()->mainCamera->transform->SetLocalTranslation(-0.005003646f, 3.973835f, -9.406748f);
	CurrentActiveScene()->mainCamera->transform->SetLocalRotation(0.094358f, -0.000349f, 0.015797f, 0.995413f);
	CurrentActiveScene()->mainCamera->SetSkybox(device, context, L"Assets/Skybox/1/Environment1HiDef.cubemap.dds", L"Assets/Skybox/1/Environment1Light.cubemap.dds");
	//CurrentActiveScene()->mainCamera->SetSkybox(device, context, L"Assets/Skybox/mp_cupertin/mp_cupertin.dds", L"Assets/Skybox/mp_cupertin/mp_cupertin_irr.dds");

	unlitShader = new SimplePixelShader(device, context);
	unlitShader->LoadShaderFile(L"UnlitMaterial.cso");

	// Add a light
	LightData light = DirectionalLight(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), 0.8f, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));;
	CurrentActiveScene()->AddLight(light);

	// Object * Cube_0 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	// Cube_0->transform->SetLocalTranslation(-2.0f, -0.5f, 0.0f);
	// Object * Cube_0_Child = Cube_0->transform->GetChildAt(0)->object;
	// MeshRenderer * meshRenderer_0 = Cube_0_Child->GetComponent<MeshRenderer>();
	// std::shared_ptr<UnlitMaterial> unlitMaterial_0 = std::make_shared<UnlitMaterial>(vertexShader, unlitShader, device);
	// unlitMaterial_0->parameters.Color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	// meshRenderer_0->SetMaterial(unlitMaterial_0);


	// Object * Cube_1 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	// Cube_1->AddComponent<MoveObject>();
	// Cube_1->transform->SetLocalTranslation(+0.0f, -0.5f, 0.0f);
	// Object * Cube_1_Child = Cube_1->transform->GetChildAt(0)->object;
	// MeshRenderer * meshRenderer_1 = Cube_1_Child->GetComponent<MeshRenderer>();
	// std::shared_ptr<UnlitMaterial> unlitMaterial_1 = std::make_shared<UnlitMaterial>(vertexShader, unlitShader, device);
	// unlitMaterial_1->parameters.Color = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	// meshRenderer_1->SetMaterial(unlitMaterial_1);

	// Object * Cube_2 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	// Cube_2->transform->SetLocalTranslation(+2.0f, -0.5f, 0.0f);
	// Object * Cube_2_Child = Cube_2->transform->GetChildAt(0)->object;
	// MeshRenderer * meshRenderer_2 = Cube_2_Child->GetComponent<MeshRenderer>();
	// std::shared_ptr<UnlitMaterial> unlitMaterial_2 = std::make_shared<UnlitMaterial>(vertexShader, unlitShader, device);
	// unlitMaterial_2->parameters.Color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	// meshRenderer_2->SetMaterial(unlitMaterial_2);

	CreateScene();

	BrickDeck::getInstance()->InitDeck(CurrentActiveScene());












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

void TowerGameApp::CreateScene()
{
	Object* TowerBase = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	TowerBase->name = "TowerBase";
	TowerBase->transform->SetLocalTranslation(+2.15f, -1.0f, 1.83f);
	TowerBase->transform->SetLocalRotation(baseRotation);
	Object* TowerBase_Child = TowerBase->transform->GetChildAt(0)->object;
	TowerBase_Child->transform->SetLocalScale(+11.18277f, +1.0f, 7.130966f);
	MeshRenderer * TowerBase_meshRenderer = TowerBase_Child->GetComponent<MeshRenderer>();
	PBRMaterial * TowerBase_Material = static_cast<PBRMaterial*>(TowerBase_meshRenderer->GetMaterial());
	//std::shared_ptr<UnlitMaterial> TowerBase_Material = std::make_shared<UnlitMaterial>(vertexShader, unlitShader, device);
	TowerBase_Material->parameters.albedo = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	Object* Decks = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	Decks->name = "Decks";
	Decks->transform->SetParent(TowerBase->transform);
	Decks->transform->SetLocalTranslation(0, 1, 0);
	Decks->transform->SetLocalRotation(DirectX::XMQuaternionIdentity());
	Object* TaskDeck = Decks->transform->GetChildAt(0)->object;
	TaskDeck->name = "TaskDeck";
	TaskDeck->transform->SetLocalTranslation(-3.289998f, -0.21f, 0.5200006f);
	TaskDeck->transform->SetLocalScale(1.800004f, 0.5f, 1.300002f);
	TaskDeck->transform->SetLocalRotation(0.000000f, 0.707107f, 0.000000f, 0.707107f);
	MeshRenderer * Decks_meshRenderer = TaskDeck->GetComponent<MeshRenderer>();
	PBRMaterial * Decks_Material = static_cast<PBRMaterial*>(Decks_meshRenderer->GetMaterial());
	Decks_Material->parameters.albedo = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);

	Object* Board = CurrentActiveScene()->AddObject("Board");
	Board->transform->SetParent(TowerBase->transform);
	Board->transform->SetLocalTranslation(0, 1, 0);
	Board->transform->SetLocalRotation(baseRotation);
	Object* Colum_0 = CurrentActiveScene()->AddObject("Colum_0");
	Colum_0->transform->SetParent(Board->transform);
	Object* Colum_1 = CurrentActiveScene()->AddObject("Colum_1");
	Colum_1->transform->SetParent(Board->transform);
	Object* Colum_2 = CurrentActiveScene()->AddObject("Colum_2");
	Colum_2->transform->SetParent(Board->transform);

	Object* PlayerHand = CurrentActiveScene()->AddObject("PlayerHand");
	PlayerHand->transform->SetParent(TowerBase->transform);
	PlayerHand->transform->SetLocalTranslation(0, 1, 0);
	PlayerHand->transform->SetLocalRotation(DirectX::XMQuaternionIdentity());
	PlayerManager::getInstance()->playerHand = PlayerHand;
	Object* BrickPosition_0 = CurrentActiveScene()->AddObject("BrickPosition_0");
	BrickPosition_0->transform->SetParent(PlayerHand->transform);
	BrickPosition_0->transform->SetLocalTranslation(3.401627f, 0.03938246f, -2.77f);
	BrickPosition_0->transform->SetLocalRotation(DirectX::XMQuaternionIdentity());
	Object* BrickPosition_1 = CurrentActiveScene()->AddObject("BrickPosition_1");
	BrickPosition_1->transform->SetParent(PlayerHand->transform);
	BrickPosition_1->transform->SetLocalTranslation(2.16f, 0.03938246f, -2.77f);
	BrickPosition_1->transform->SetLocalRotation(DirectX::XMQuaternionIdentity());
	Object* BrickPosition_2 = CurrentActiveScene()->AddObject("BrickPosition_2");
	BrickPosition_2->transform->SetParent(PlayerHand->transform);
	BrickPosition_2->transform->SetLocalTranslation(4.61f, 0.03938246f, -2.77f);
	BrickPosition_2->transform->SetLocalRotation(DirectX::XMQuaternionIdentity());

	Object* CardPosition_0 = CurrentActiveScene()->AddObject("CardPosition_0");
	CardPosition_0->transform->SetParent(CurrentActiveScene()->mainCamera->transform);
	CardPosition_0->transform->SetLocalTranslation(-0.2964f, -0.1171f, 0.35f);
	CardPosition_0->transform->SetLocalRotation(0.005277f, -0.051002f, 0.102791f, 0.993381f);
	Object* CardPosition_1 = CurrentActiveScene()->AddObject("CardPosition_1");
	CardPosition_1->transform->SetParent(CurrentActiveScene()->mainCamera->transform);
	CardPosition_1->transform->SetLocalTranslation(-0.176f, -0.104f, 0.352f);
	CardPosition_1->transform->SetLocalRotation(0.000000f, -0.029007f, 0.000000f, 0.999579f);
	Object* CardPosition_2 = CurrentActiveScene()->AddObject("CardPosition_2");
	CardPosition_2->transform->SetParent(CurrentActiveScene()->mainCamera->transform);
	CardPosition_2->transform->SetLocalTranslation(-0.0602f, -0.1108f, 0.3499f);
	CardPosition_2->transform->SetLocalRotation(-0.000772f, -0.009081f, -0.084690f, 0.996366f);




	Object* testCube_0 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	testCube_0->transform->SetParent(BrickPosition_0->transform);
	testCube_0->transform->SetLocalTranslation(0, 0, 0);
	testCube_0->transform->SetLocalRotation(DirectX::XMQuaternionIdentity());
	MeshRenderer* cube0Renderer = testCube_0->transform->GetChildAt(0)->object->GetComponent<MeshRenderer>();
	PBRMaterial* cube0Material = static_cast<PBRMaterial*>(cube0Renderer->GetMaterial());
	cube0Material->parameters.albedo = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	cube0Material->parameters.roughness = 0.0f;
	cube0Material->parameters.metalness = 1.0f;
	Object* testCube_1 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	testCube_1->transform->SetParent(BrickPosition_1->transform);
	testCube_1->transform->SetLocalTranslation(0, 0, 0);
	testCube_1->transform->SetLocalRotation(DirectX::XMQuaternionIdentity());
	MeshRenderer* cube1Renderer = testCube_1->transform->GetChildAt(0)->object->GetComponent<MeshRenderer>();
	PBRMaterial* cube1Material = static_cast<PBRMaterial*>(cube1Renderer->GetMaterial());
	cube1Material->parameters.albedo = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
	cube1Material->parameters.roughness = 0.0f;
	cube1Material->parameters.metalness = 1.0f;
	Object* testCube_2 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	testCube_2->transform->SetParent(BrickPosition_2->transform);
	testCube_2->transform->SetLocalTranslation(0, 0, 0);
	testCube_2->transform->SetLocalRotation(DirectX::XMQuaternionIdentity());
	MeshRenderer* cube2Renderer = testCube_2->transform->GetChildAt(0)->object->GetComponent<MeshRenderer>();
	PBRMaterial* cube2Material = static_cast<PBRMaterial*>(cube2Renderer->GetMaterial());
	cube2Material->parameters.albedo = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	cube2Material->parameters.roughness = 0.0f;
	cube2Material->parameters.metalness = 1.0f;

	Object* testCard_0 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	testCard_0->transform->SetParent(CardPosition_0->transform);
	testCard_0->transform->SetLocalTranslation(0, 0, 0);
	testCard_0->transform->SetLocalRotation(DirectX::XMQuaternionIdentity());
	testCard_0->transform->SetLocalScale(0.1342f, 0.1844f, 0.01f);
	Object* testCard_1 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	testCard_1->transform->SetParent(CardPosition_1->transform);
	testCard_1->transform->SetLocalTranslation(0, 0, 0);
	testCard_1->transform->SetLocalRotation(DirectX::XMQuaternionIdentity());
	testCard_1->transform->SetLocalScale(0.1342f, 0.1844f, 0.01f);
	Object* testCard_2 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	testCard_2->transform->SetParent(CardPosition_2->transform);
	testCard_2->transform->SetLocalTranslation(0, 0, 0);
	testCard_2->transform->SetLocalRotation(DirectX::XMQuaternionIdentity());
	testCard_2->transform->SetLocalScale(0.1342f, 0.1844f, 0.01f);
}