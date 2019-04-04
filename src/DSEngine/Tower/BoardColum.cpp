#include "BoardColum.h"
#include "Object.hpp"
#include <DirectXMath.h>

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
    bricks[brickCount] = brick;

    //Visual effect of BRICK placement
    brick->object->transform->SetLocalTranslation(DirectX::XMVectorAdd(object->transform->GetLocalTranslation(), DirectX::XMVectorSet(0, brickHeight * brickCount, 0, 0)));
    brick->OnPlaced();
    ++brickCount;
    //if (brickCount >= BoardManager::getInstance()->maxBrickPerColum) {
    //    isFull = true;
    //}
}