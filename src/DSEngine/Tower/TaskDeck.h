#pragma once
#include "TaskCard.h"
#include "Scene.hpp"
class TaskDeck
{
private:
	static TaskDeck* instance;
	int TotalNum = 0;
public:
	static TaskDeck* getInstance();
	std::vector<TaskCard> tasks;
	void Shuffle();
	void InitDeck(Scene* scene);
	Scene* scene;
	TaskCard DrawTask();
	TaskDeck();
	~TaskDeck();
};
