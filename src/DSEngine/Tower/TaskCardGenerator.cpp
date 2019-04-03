#include "TaskCardGenerator.h"



TaskCardGenerator::TaskCardGenerator()
{
}


TaskCardGenerator::~TaskCardGenerator()
{
}

Object* TaskCardGenerator::AddTaskCard(Scene* scene, TaskCardData data, std::string name)
{
	Object* taskCard = scene->AddObject(name);





	return taskCard;
}