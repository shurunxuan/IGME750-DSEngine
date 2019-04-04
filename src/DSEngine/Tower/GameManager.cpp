#include "GameManager.h"
#include "TaskDeck.h"
#include "PlayerManager.h"
GameManager* GameManager::instance = 0;

GameManager* GameManager::getInstance()
{
	return instance;
}

GameManager::GameManager()
{
	instance = this;
	Update(Initial);
}


GameManager::~GameManager()
{
}

void GameManager::Update(GameState gs)
{
	switch (gs)
	{
	case Initial:		
		TaskDeck::getInstance()->InitDeck();
		//BrickDeck::getInstance()->InitDeck();
		currentGameState = Idle;
		break;
	case Idle:
		break;
	case BrickSelected:
		break;
	case DrawBrick:
		PlayerManager::getInstance()->DrawBrick(selectedBrick->object->transform->GetGlobalTranslation());
		break;
	case DrawTask:
		if (ContinousDrawTask)
		{
			PlayerManager::getInstance()->DrawTaskCard();
			ContinousDrawTask = false;
		}
		currentGameState = Idle;
		break;
	case End:
		break;
	default:
		break;
	}
}

void GameManager::CheckTaskCompletion()
{
	std::vector<int> completed;
	for (int n = PlayerManager::getInstance()->tasksInHand.size() - 1; n >= 0; n--)
	{	
		std::vector<BrickMatrix> matrices = BoardManager::getInstance()->GetBrickMatrices(PlayerManager::getInstance()->tasksInHand[n]);
		for(size_t index = 0; index < matrices.size(); index++)
		{

			int result = 0;
			bool cells[3][3];

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					cells[2 - i][j] = PlayerManager::getInstance()->tasksInHand[n].GetTaskData().pattern[i * 3 + j];
				}
			}

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (cells[i][j])
					{
						result += static_cast<int>(matrices[index].bricks[i][j]);
					}
				}
			}

			//If value is equal, complete this TASK
			if (result == PlayerManager::getInstance()->tasksInHand[n].GetColorValue())
			{
				completed.push_back(n);
			}
		}
	}

	PlayerManager::getInstance()->CompleteTask(completed);
}
