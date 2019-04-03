#include "TowerGameApp.h"
#include "CSVReader.h"
#include "MoveObject.h"
#include "UnlitMaterial.h"

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
	SInput->RegisterInput("Horizontal", "d", "a", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("Horizontal", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Axis, JoystickLX, -1);
	SInput->RegisterInput("Vertical", "w", "s", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);
	SInput->RegisterInput("Vertical", "", "", "", "", 10.0f, 0.1f, 10.0f, false, Axis, JoystickLY, -1);
	SInput->RegisterInput("Test", "mouse 0", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);

	CurrentActiveScene()->mainCamera->UpdateProjectionMatrix(float(width), float(height), DirectX::XM_PIDIV4);
	//CurrentActiveScene()->mainCamera->transform->SetLocalTranslation(0.0f, 0.0f, 0.0f);
	CurrentActiveScene()->mainCamera->transform->SetLocalTranslation(-0.005003646f, 3.973835f, -9.406748f);
	CurrentActiveScene()->mainCamera->transform->SetLocalRotation(0.094358f, -0.000349f, 0.015797f, 0.995413f);
	//CurrentActiveScene()->mainCamera->SetSkybox(device, context, L"Assets/Skybox/1/Environment1HiDef.cubemap.dds", L"Assets/Skybox/1/Environment1Light.cubemap.dds");
	CurrentActiveScene()->mainCamera->SetSkybox(device, context, L"Assets/Skybox/mp_cupertin/mp_cupertin.dds", L"Assets/Skybox/mp_cupertin/mp_cupertin_irr.dds");

	unlitShader = new SimplePixelShader(device, context);
	unlitShader->LoadShaderFile(L"UnlitMaterial.cso");

	// Add a light
	LightData light = DirectionalLight(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f), 0.8f, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));;
	CurrentActiveScene()->AddLight(light);

	Object * Cube_0 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	Cube_0->transform->SetLocalTranslation(-2.0f, -0.5f, 0.0f);
	Object * Cube_0_Child = Cube_0->transform->GetChildAt(0)->object;
	MeshRenderer * meshRenderer_0 = Cube_0_Child->GetComponent<MeshRenderer>();
	std::shared_ptr<UnlitMaterial> unlitMaterial_0 = std::make_shared<UnlitMaterial>(vertexShader, unlitShader, device);
	unlitMaterial_0->parameters.Color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	meshRenderer_0->SetMaterial(unlitMaterial_0);


	Object * Cube_1 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	Cube_1->AddComponent<MoveObject>();
	Cube_1->transform->SetLocalTranslation(+0.0f, -0.5f, 0.0f);
	Object * Cube_1_Child = Cube_1->transform->GetChildAt(0)->object;
	MeshRenderer * meshRenderer_1 = Cube_1_Child->GetComponent<MeshRenderer>();
	std::shared_ptr<UnlitMaterial> unlitMaterial_1 = std::make_shared<UnlitMaterial>(vertexShader, unlitShader, device);
	unlitMaterial_1->parameters.Color = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	meshRenderer_1->SetMaterial(unlitMaterial_1);

	Object * Cube_2 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	Cube_2->transform->SetLocalTranslation(+2.0f, -0.5f, 0.0f);
	Object * Cube_2_Child = Cube_2->transform->GetChildAt(0)->object;
	MeshRenderer * meshRenderer_2 = Cube_2_Child->GetComponent<MeshRenderer>();
	std::shared_ptr<UnlitMaterial> unlitMaterial_2 = std::make_shared<UnlitMaterial>(vertexShader, unlitShader, device);
	unlitMaterial_2->parameters.Color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	meshRenderer_2->SetMaterial(unlitMaterial_2);

	Object * TowerBase = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	TowerBase->name = "TowerBase";
	TowerBase->transform->SetLocalTranslation(+2.15f, -1.0f, 1.83f);
	TowerBase->transform->SetLocalRotation(baseRotation);
	Object * TowerBase_Child = TowerBase->transform->GetChildAt(0)->object;
	TowerBase_Child->transform->SetLocalScale(+11.18277f, +1.0f, 7.130966f);
	MeshRenderer * TowerBase_meshRenderer = TowerBase_Child->GetComponent<MeshRenderer>();
	PBRMaterial * TowerBase_Material = static_cast<PBRMaterial*>(TowerBase_meshRenderer->GetMaterial());
	//std::shared_ptr<UnlitMaterial> TowerBase_Material = std::make_shared<UnlitMaterial>(vertexShader, unlitShader, device);
	TowerBase_Material->parameters.albedo = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	Object * Decks = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	Decks->name = "Decks";
	Decks->transform->SetParent(TowerBase->transform);
	Decks->transform->SetLocalTranslation(0, 1, 0);
	Decks->transform->SetLocalRotation(DirectX::XMQuaternionIdentity());
	Object * Decks_Child = Decks->transform->GetChildAt(0)->object;
	Decks_Child->name = "TaskDeck";
	Decks_Child->transform->SetLocalTranslation(-3.289998f, -0.21f, 0.5200006f);
	Decks_Child->transform->SetLocalScale(1.800004f, 0.5f, 1.300002f);
	Decks_Child->transform->SetLocalRotation(0.000000f, 0.707107f, 0.000000f, 0.707107f);
	MeshRenderer * Decks_meshRenderer = Decks_Child->GetComponent<MeshRenderer>();
	PBRMaterial * Decks_Material = static_cast<PBRMaterial*>(Decks_meshRenderer->GetMaterial());
	Decks_Material->parameters.albedo = DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f);

	CSVReader reader;
	LOG_TRACE << "ReadFile";
	vector<TaskCardData> data = reader.ReadFromFile("TaskCardData.csv");












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
