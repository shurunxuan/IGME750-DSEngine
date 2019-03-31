#include "TowerGameApp.h"
#include "UnlitMaterial.h"

#include "RaycastTest.h"

#include "MoveObject.h"


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
	SInput->RegisterInput("Test", "space", "", "", "", 10.0f, 0.1f, 10.0f, false, Button, MouseX, -1);

	CurrentActiveScene()->mainCamera->UpdateProjectionMatrix(float(width), float(height), DirectX::XM_PIDIV4);
	//CurrentActiveScene()->mainCamera->transform->SetLocalTranslation(0.0f, 0.0f, 0.0f);
	CurrentActiveScene()->mainCamera->transform->SetLocalTranslation(-0.005003646f, 3.973835f, -9.406748f);
	CurrentActiveScene()->mainCamera->transform->SetLocalRotation(
		DirectX::XMQuaternionRotationRollPitchYaw(
			DirectX::XMConvertToRadians(10.828f), 
			DirectX::XMConvertToRadians(0.133f), 
			DirectX::XMConvertToRadians(1.831f)));

	unlitShader = new SimplePixelShader(device, context);
	unlitShader->LoadShaderFile(L"UnlitMaterial.cso");

	Object* Cube_0 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	Cube_0->transform->SetLocalTranslation(-2.0f, -0.5f, 0.0f);
	Object* Cube_0_Child = Cube_0->transform->GetChildAt(0)->object;
	MeshRenderer* meshRenderer_0 = Cube_0_Child->GetComponent<MeshRenderer>();
	std::shared_ptr<UnlitMaterial> unlitMaterial_0 = std::make_shared<UnlitMaterial>(vertexShader, unlitShader, device);
	unlitMaterial_0->parameters.Color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	meshRenderer_0->SetMaterial(unlitMaterial_0);
	BoxCollider* collider_0 = Cube_0->AddComponent<BoxCollider>();
	collider_0->GetCollider()->Transform(*(collider_0->GetCollider()), 1.0f, DirectX::XMQuaternionIdentity(), DirectX::XMVectorSet(-2.0f, -0.5f, 0.0f, 0.0f));


	Object* Cube_1 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	Cube_1->AddComponent<MoveObject>();
	Cube_1->transform->SetLocalTranslation(+0.0f, -0.5f, 0.0f);
	Object* Cube_1_Child = Cube_1->transform->GetChildAt(0)->object;
	MeshRenderer* meshRenderer_1 = Cube_1_Child->GetComponent<MeshRenderer>();
	std::shared_ptr<UnlitMaterial> unlitMaterial_1 = std::make_shared<UnlitMaterial>(vertexShader, unlitShader, device);
	unlitMaterial_1->parameters.Color = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	meshRenderer_1->SetMaterial(unlitMaterial_1);
	BoxCollider* collider_1 = Cube_1->AddComponent<BoxCollider>();
	collider_1->GetCollider()->Transform(*(collider_1->GetCollider()), 1.0f, DirectX::XMQuaternionIdentity(), DirectX::XMVectorSet(+0.0f, -0.5f, 0.0f, 0.0f));

	Object* Cube_2 = CurrentActiveScene()->LoadModelFile("Assets/Models/cube.obj");
	Cube_2->transform->SetLocalTranslation(+2.0f, -0.5f, 0.0f);
	Object* Cube_2_Child = Cube_2->transform->GetChildAt(0)->object;
	MeshRenderer* meshRenderer_2 = Cube_2_Child->GetComponent<MeshRenderer>();
	std::shared_ptr<UnlitMaterial> unlitMaterial_2 = std::make_shared<UnlitMaterial>(vertexShader, unlitShader, device);
	unlitMaterial_2->parameters.Color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	meshRenderer_2->SetMaterial(unlitMaterial_2);

	BoxCollider* collider_2 = Cube_2->AddComponent<BoxCollider>();
	collider_2->GetCollider()->Transform(*(collider_2->GetCollider()), 1.0f, DirectX::XMQuaternionIdentity(), DirectX::XMVectorSet(+2.0f, -0.5f, 0.0f, 0.0f));

	Object* test = CurrentActiveScene()->AddObject("test");
	RaycastTest* raycastTest = Cube_0->AddComponent<RaycastTest>();
	CurrentActiveScene()->DestroyObject(test);
	CurrentActiveScene()->DestroyObject(Cube_2);


	LOG_INFO << MoveObject::instance->object->name << " is get from instance";

}
