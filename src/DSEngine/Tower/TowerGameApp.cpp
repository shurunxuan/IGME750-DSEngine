#include "TowerGameApp.h"
#include "CSVReader.h"
#include "UnlitMaterial.h"

TowerGameApp::~TowerGameApp()
{
	delete unlitShader;
}

void TowerGameApp::Init()
{
	CurrentActiveScene()->mainCamera->UpdateProjectionMatrix(float(width), float(height), DirectX::XM_PIDIV4);
	CurrentActiveScene()->mainCamera->transform->SetLocalTranslation(0.0f, 0.0f, -10.0f);

	unlitShader = new SimplePixelShader(device, context);
	unlitShader->LoadShaderFile(L"UnlitMaterial.cso");

	Object* Cube_0 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	Cube_0->transform->SetLocalTranslation(-2.0f, -0.5f, 0.0f);
	Object* Cube_0_Child = Cube_0->transform->GetChildAt(0)->object;
	MeshRenderer* meshRenderer_0 = Cube_0_Child->GetComponent<MeshRenderer>();
	std::shared_ptr<UnlitMaterial> unlitMaterial_0 = std::make_shared<UnlitMaterial>(vertexShader, unlitShader, device);
	unlitMaterial_0->parameters.Color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	meshRenderer_0->SetMaterial(unlitMaterial_0);


	Object* Cube_1 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	Cube_1->transform->SetLocalTranslation(+0.0f, -0.5f, 0.0f);
	Object* Cube_1_Child = Cube_1->transform->GetChildAt(0)->object;
	MeshRenderer* meshRenderer_1 = Cube_1_Child->GetComponent<MeshRenderer>();
	std::shared_ptr<UnlitMaterial> unlitMaterial_1 = std::make_shared<UnlitMaterial>(vertexShader, unlitShader, device);
	unlitMaterial_1->parameters.Color = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	meshRenderer_1->SetMaterial(unlitMaterial_1);

	Object* Cube_2 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	Cube_2->transform->SetLocalTranslation(+2.0f, -0.5f, 0.0f);
	Object* Cube_2_Child = Cube_2->transform->GetChildAt(0)->object;
	MeshRenderer* meshRenderer_2 = Cube_2_Child->GetComponent<MeshRenderer>();
	std::shared_ptr<UnlitMaterial> unlitMaterial_2 = std::make_shared<UnlitMaterial>(vertexShader, unlitShader, device);
	unlitMaterial_2->parameters.Color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	meshRenderer_2->SetMaterial(unlitMaterial_2);

	CSVReader reader;
	LOG_TRACE << "ReadFile" << '\n';
	vector<TaskCardData> data = reader.ReadFromFile("TaskCardData.csv");
	
}
