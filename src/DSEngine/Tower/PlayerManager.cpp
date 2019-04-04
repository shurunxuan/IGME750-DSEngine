#include "PlayerManager.h"

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
		//UIUPdateHand()
	}
	GameManager::getInstance()->currentGameState = Idle;
}

void PlayerManager::DrawBrick(DirectX::XMVECTOR position)
{
	bricksInHand.push_back(BrickDeck::getInstance()->OnDraw(position));
}

void PlayerManager::UseBrick(Brick* usedBrick)
{
	auto it = std::find(bricksInHand.begin(), bricksInHand.end(), usedBrick);
	if (it != bricksInHand.end())
		bricksInHand.erase(it);
}

void PlayerManager::CompleteTask(std::vector<int> index)
{
	for (int i : index) {
		score += tasksInHand[i].GetTaskData().Score;
		LOG_DEBUG << "Score: "<<score;

		tasksInHand.erase(tasksInHand.begin() + i);
		taskCount--;
	}
	//update ui
}

PlayerManager::PlayerManager()
{
	instance = this;
}


PlayerManager::~PlayerManager()
{
}
