#include "TaskDeck.h"

TaskDeck* TaskDeck::instance = 0;

TaskDeck* TaskDeck::getInstance()
{
	return instance;
}

void TaskDeck::Shuffle()
{
	std::vector<TaskCard> tempTasks;
	for(int i=0; i < TotalNum; i++)
	{
		int index = rand() % tasks.size();
		tempTasks.push_back(tasks[index]);
		tasks.erase(tasks.begin()+index);
	}
	tasks = tempTasks;
}

void TaskDeck::InitDeck()
{
	CSVReader reader;
	std::vector<TaskCardData> data = reader.ReadFromFile("TaskCardData.csv");
	for (int i = 0; i < data.size(); i++)
	{
		TaskCard c(data[i]);
		tasks.push_back(c);
	}
}

TaskCard TaskDeck::DrawTask()
{
	int index = rand() % TotalNum;
	TaskCard task = tasks[index];
	tasks.erase(tasks.begin()+index);
	TotalNum--;
	return task;
}

TaskDeck::TaskDeck()
{
	instance = this;
}

TaskDeck::~TaskDeck() 
{
}