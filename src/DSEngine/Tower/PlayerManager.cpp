#include "PlayerManager.h"
#include "TaskCardGenerator.h"

PlayerManager* PlayerManager::instance = nullptr;

PlayerManager* PlayerManager::getInstance()
{
	return instance;
}

void PlayerManager::AddTask(TaskCard task)
{
	tasksInHand.push_back(task);
	taskCount++;
}

void PlayerManager::DrawTaskCard()
{
	if (tasksInHand.size() < 3)
	{
		tasksInHand.push_back(TaskDeck::getInstance()->DrawTask());
		UIUpdateHand();
	}
	GameManager::getInstance()->currentGameState = Idle;
}

void PlayerManager::DrawBrick(Transform* transform)
{
	if (lastDeleted < 0)
		bricksInHand.push_back(BrickDeck::getInstance()->OnDraw(transform));
	else
	{

		bricksInHand.emplace(bricksInHand.begin() + lastDeleted, BrickDeck::getInstance()->OnDraw(transform));

	}
}

void PlayerManager::UseBrick(Brick* usedBrick)
{
	auto it = std::find(bricksInHand.begin(), bricksInHand.end(), usedBrick);
	if (it != bricksInHand.end())
	{
		lastDeleted = int(it - bricksInHand.begin());
		bricksInHand.erase(it);
	}
}

void PlayerManager::CompleteTask(std::vector<int> index)
{
	for (int i : index) {
		score += tasksInHand[i].GetTaskData().Score;
		LOG_DEBUG << "Score: " << score;

		tasksInHand.erase(tasksInHand.begin() + i);
		taskCount--;
	}
	
	UIUpdateHand();
}

PlayerManager::PlayerManager()
{
	instance = this;
	for (int i = 0; i < 3; i++)
	{
		UItaskInHand[i] = nullptr;
	}
}


PlayerManager::~PlayerManager()
{
}

void PlayerManager::UIUpdateHand()
{
	for (int n = 0; n < 3; n++)
	{
		if (UItaskInHand[n] != nullptr)
		{
			UItaskInHand[n]->transform->SetLocalTranslation(0, 0, -100);
			UItaskInHand[n] = nullptr;
		}
	}
	int i = 0;
	for(auto t : tasksInHand)
	{
		UItaskInHand[i] = TaskCardGenerator::AddTaskCard(TaskDeck::getInstance()->scene, t.GetTaskData(), taskPos[i]->transform);
		i++;
	}
}