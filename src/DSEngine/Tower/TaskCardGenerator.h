#pragma once
#include <string>
#include "CSVReader.h"
#include "Object.hpp"
#include "Scene.hpp"

class TaskCardGenerator
{
public:
    static Object* AddTaskCard(Scene* scene, TaskCardData data, Transform* parent, std::string name = "TaskCard");
	TaskCardGenerator();
	~TaskCardGenerator();
};

