#include "TaskCardGenerator.h"



TaskCardGenerator::TaskCardGenerator()
{
}


TaskCardGenerator::~TaskCardGenerator()
{
}

Object* TaskCardGenerator::AddTaskCard(Scene* scene, TaskCardData data, Transform* parent, std::string name)
{
	Object* taskCard = scene->LoadModelFile("Assets/Models/cube.obj");
	taskCard->name = name;
	taskCard->transform->SetParent(parent);
	taskCard->transform->SetLocalTranslation(0.0f, 0.0f, 0.0f);
	taskCard->transform->SetLocalRotation(DirectX::XMQuaternionIdentity());
	taskCard->transform->SetLocalScale(0.1342f, 0.1844f, 0.01f);

	std::stringstream ss;
	ss << "Assets/Textures/Cards/";
	ss << data.Score;
	for (int i : data.pattern)
		ss << i;
	for (Color c : data.color)
	{
		switch (c) {
		case Blue: ss << "Blue"; break;
		case Yellow: ss << "Yellow"; break;
		case Red: ss << "Red"; break;
		default:;
		}
	}
	ss << ".png";
	std::string taskCardTexture;
	ss >> taskCardTexture;

	MeshRenderer* taskCardMR = taskCard->transform->GetChildAt(0)->object->GetComponent<MeshRenderer>();
	PBRMaterial* taskCardPBR = static_cast<PBRMaterial*>(taskCardMR->GetMaterial());
	taskCardPBR->LoadDiffuseTexture(taskCardTexture);
	taskCardPBR->parameters.metalness = 0.0f;
	taskCardPBR->parameters.roughness = 1.0f;

	return taskCard;
}