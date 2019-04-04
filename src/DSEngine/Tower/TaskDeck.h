#pragma once
#include "TaskCard.h"
class TaskDeck
{
private:
	static TaskDeck* instance;
	int TotalNum = 0;
public:
	static TaskDeck* getInstance();
	std::vector<TaskCard> tasks;
	void Shuffle();
	void InitDeck();
	TaskCard DrawTask();
	TaskDeck();
	~TaskDeck();
};
