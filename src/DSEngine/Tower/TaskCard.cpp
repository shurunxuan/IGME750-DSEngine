#include "TaskCard.h"
TaskCard::TaskCard(TaskCardData data)
{
	taskData = data;
}

int TaskCard::GetColorValue()
{
	int colorValue = 0;
	for (int i = 0; i < taskData.color.size(); i++)
	{
		colorValue += (int)taskData.color[i];
	}
	return colorValue;
}

TaskCardData TaskCard::GetTaskData()
{
	return taskData;
}

void TaskCard::OnCompleted()
{
	isCompleted = true;
}


TaskCard::~TaskCard()
{
}
