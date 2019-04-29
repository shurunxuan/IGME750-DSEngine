#include "TaskCard.h"
TaskCard::TaskCard(TaskCardData data)
{
	taskData = data;
}

int TaskCard::GetColorValue()
{
	int colorValue = 0;
	for (auto& i : taskData.color)
	{
		colorValue += static_cast<int>(i);
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
