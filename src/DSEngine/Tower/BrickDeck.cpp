#include "BrickDeck.h"
#include "Brick.h"
#include "GameManager.h"
#include "PlayerManager.h"
#include "Scene.hpp"

BrickDeck* BrickDeck::instance = 0;

BrickDeck* BrickDeck::getInstance()
{
	return instance;
}

BrickDeck::BrickDeck()
{
    instance = this;
}


BrickDeck::~BrickDeck()
{
}

Brick* BrickDeck::OnDraw(DirectX::XMVECTOR position)
{
    int index = rand() % bricks.size();
	auto it = bricks.begin();
	while (index > 0)
	{
		++it;
		--index;
	}
    Brick* brick = *it;
    bricks.erase(it);

    if (GameManager::getInstance()->currentGameState != Initial)
    {
        position = DirectX::XMVectorSubtract(position, DirectX::XMVectorSet(0.0f, 0.5f, 0.0f, 0.0f));
    }

	brick->object->transform->SetLocalTranslation(position);
    brick->object->transform->SetParent(PlayerManager::getInstance()->playerHand->transform);

    return brick;
}

void BrickDeck::InitDeck(Scene* scene)
{
    for (int i = 0; i < YellowBrickNum; i++)
    {
        Brick* brick = CreateBrick(scene, YellowBrick, DirectX::XMVectorSet(0, -100, 0, 1));
        bricks.push_back(brick);
    }
    for (int i = 0; i < BlueBrickNum; i++)
    {
        Brick* brick = CreateBrick(scene, BlueBrick, DirectX::XMVectorSet(0, -100, 0, 1));
        bricks.push_back(brick);
    }
    for (int i = 0; i < RedBrickNum; i++)
    {
        Brick* brick = CreateBrick(scene, RedBrick, DirectX::XMVectorSet(0, -100, 0, 1));
        bricks.push_back(brick);
    }
}

Brick* BrickDeck::CreateBrick(Scene* scene, BrickColor color, DirectX::XMVECTOR position)
{
	Object* brickObject = scene->LoadModelFile("Assets/Models/cube.obj");
	brickObject->transform->SetLocalTranslation(position);
	MeshRenderer* renderer = brickObject->transform->GetChildAt(0)->object->GetComponent<MeshRenderer>();
	PBRMaterial* material = static_cast<PBRMaterial*>(renderer->GetMaterial());
    material->parameters.roughness = 0.0f;
	material->parameters.metalness = 1.0f;

	Brick* brick = brickObject->AddComponent<Brick>();

    switch (color) {
        case RedBrick:
        	material->parameters.albedo = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
            brick->SetBrickColor(1);
            break;
        case YellowBrick:
        	material->parameters.albedo = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
            brick->SetBrickColor(2);
            break;
        case BlueBrick:
        	material->parameters.albedo = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
            brick->SetBrickColor(0);
            break; 
    }

    return brick;
}
