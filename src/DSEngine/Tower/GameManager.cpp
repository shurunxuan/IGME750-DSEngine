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
		if (ContinousDrawTask == true)
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
	//std::vector<int> completed;
	//for (int n = PlayerManager::getInstance()->tasksInHand.size() - 1; n >= 0; n--)
	//{	
	//	std::vector<BrickMatrix> matrices = BoardManager::getInstance()->
	//	List<BrickMatrix> matrices = BoardManager.Instance.GetBrickMatrices(PlayerManager.Instance.tasksInHand[n]);
	//	foreach(BrickMatrix matrix in matrices)
	//	{

	//		int result = 0;
	//		bool[, ] cells = new bool[3, 3];

	//		for (int i = 0; i < 3; i++)
	//		{
	//			for (int j = 0; j < 3; j++)
	//			{
	//				cells[2 - i, j] = PlayerManager.Instance.tasksInHand[n].taskData.GetCells()[i, j];
	//			}
	//		}

	//		for (int i = 0; i < 3; i++)
	//		{
	//			for (int j = 0; j < 3; j++)
	//			{
	//				if (cells[i, j])
	//				{
	//					result += (int)matrix.bricks[i, j];
	//				}
	//			}
	//		}

	//		//If value is equal, complete this TASK
	//		if (result == PlayerManager.Instance.tasksInHand[n].GetColorValue())
	//		{
	//			completed.Add(n);
	//		}
	//	}
	//}

	//PlayerManager.Instance.CompleteTask(completed);
}
