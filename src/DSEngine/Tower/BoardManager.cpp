#include "BoardManager.h"
#include "PlayerManager.h"

BoardManager* BoardManager::instance = 0;

BoardManager* BoardManager::getInstance()
{
	return instance;
}

BoardManager::BoardManager()
{
	instance = this;
}


BoardManager::~BoardManager()
{
}

bool BoardManager::PlaceBrickAt(Brick* brick, BoardColum* colum)
{
	if (!colum->isFull)
	{
		PlayerManager::getInstance()->UseBrick(brick);
		PlayerManager::getInstance()->DrawBrick(brick->object->transform->GetGlobalTranslation());
		colum->PlaceBrick(brick);

		row = colum->brickCount - 1;
		col = colum->index;
	}
	else
	{
		return false;
	}

	if (++brickCount >= maxBrickCount)
	{
		OnFull();
	}

	GameManager::getInstance()->currentGameState = End;
	return true;
}

std::vector<BrickMatrix> BoardManager::GetBrickMatrices(TaskCard task)
{
	std::vector<BrickMatrix> matrices;
	bool cells[3][3];

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cells[2 - i][j] = task.GetTaskData().pattern[i * 3 + j];
		}
	}

	int r = row;
	int c = col;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (cells[i, j])
			{
				int x = r - i;
				int y = c - j;

				BrickMatrix m;

				for (int a = 0; a < 3; a++)
				{
					for (int b = 0; b < 3; b++)
					{
						int p = x + a;
						int q = y + b;

						if (q < 0 || q > 2 || p < 0 || p >= maxBrickPerColum)
						{
							m.bricks[a][b] = Error;
						}
						else
						{
							Brick* brick = GetBrickAt(p, q);
							if (brick != nullptr)
								m.bricks[a][b] = brick->color;
							else
								m.bricks[a][b] = Empty;
						}
					}
				}
				matrices.push_back(m);
			}
		}
	}
	return matrices;
}

void BoardManager::OnFull()
{
	//End the game
	//Calculate the points and display it
	//display game ends
}

Brick* BoardManager::GetBrickAt(int r, int c)
{
	if (c >= 3 || c < 0 || r < 0 || r >= boardColums[c]->brickCount)
	{
		return nullptr;
	}
	return boardColums[c]->bricks[r];
}
