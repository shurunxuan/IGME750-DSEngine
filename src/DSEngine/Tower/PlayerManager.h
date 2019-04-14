#pragma once
#include "TaskCard.h"
#include "Object.hpp"
#include "BrickDeck.h"
#include "TaskDeck.h"
#include "GameManager.h"

class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager();
	static PlayerManager* getInstance();
	std::vector<TaskCard> tasksInHand;
	std::vector<Brick*> bricksInHand;
	void AddTask(TaskCard task);
	void DrawTaskCard();
	void DrawBrick(Transform* transform);
	void UseBrick(Brick* usedBrick);
	void CompleteTask(std::vector<int> index);
	void UIUpdateHand();
	Object* playerHand;
	Object* taskPos[3];
	Object* brickPositions[3];
	Object* UItaskInHand[3];
	int score = 0;
	int taskCount = 0;
private:
	static PlayerManager* instance;
	int lastDeleted = -1;
};

