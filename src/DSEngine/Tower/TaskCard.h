#pragma once
#include "CSVReader.h"

class TaskCard
{
public:
	TaskCard(TaskCardData);
	int GetColorValue();
	TaskCardData GetTaskData();
	void OnCompleted();
	bool isCompleted;
	~TaskCard();
private:
	TaskCardData taskData;
};

