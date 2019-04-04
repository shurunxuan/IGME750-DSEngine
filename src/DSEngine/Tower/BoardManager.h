#pragma once
#include "Brick.h"
#include "BoardColum.h"
#include "TaskCard.h"

struct BrickMatrix {
	BrickColor bricks[3][3];
};

class BoardManager
{
public:
	static BoardManager* getInstance();
	BoardManager();
	~BoardManager();

	int maxBrickPerColum = 8;
	float brickHeight = 1.0f;

	BoardColum* boardColums[3];

	bool PlaceBrickAt(Brick* brick, BoardColum* colum);
	std::vector<BrickMatrix> GetBrickMatrices(TaskCard task);
	Brick* GetBrickAt(int r, int c);


private:
	static BoardManager* instance;

	int maxBrickCount = 24;
	int brickCount = 0;

	int row;
	int col;

	void OnFull();

};

