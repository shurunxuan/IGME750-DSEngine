#include "BoardColum.h"
#include "Object.hpp"
#include <DirectXMath.h>
#include "BoardManager.h"

BoardColum::BoardColum(Object* owner)
	: Component(owner)
{
}


BoardColum::~BoardColum()
{
}

void BoardColum::Start() 
{
    // GetComponent<MeshRenderer>().enabled = false;
    // bricks = new Brick[BoardManager.Instance.maxBrickPerColum];

    // brickHeight = BoardManager.Instance.brickHeight;
}

void BoardColum::Update(float deltaTime, float totalTime) {}

void BoardColum::PlaceBrick(Brick* brick) 
{
    bricks.push_back(brick);

    //Visual effect of BRICK placement
    brick->object->transform->SetLocalTranslation(DirectX::XMVectorAdd(object->transform->GetLocalTranslation(), DirectX::XMVectorSet(0, (brickHeight + 0.2f) * brickCount, 0, 0)));
	//brick->object->transform->SetLocalTranslation(position);
	//brick->object->transform->SetLocalRotation(DirectX::XMQuaternionIdentity());
	brick->OnPlaced();
    ++brickCount;
    if (brickCount >= BoardManager::getInstance()->maxBrickPerColum) {
        isFull = true;
    }
}