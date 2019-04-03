#include "BoardColum.h"



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
    //brick->transform.position = transform.position + new Vector3(0, brickHeight * brickCount, 0);
    //brick.OnPlaced();
    //++brickCount;
    //if (brickCount >= BoardManager.Instance.maxBrickPerColum) {
    //    isFull = true;
    //}
}