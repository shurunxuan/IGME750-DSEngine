#pragma once
#include "TaskCard.h"
#include "Object.hpp"
class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager();
	static PlayerManager* getInstance();
	std::vector<TaskCard> taskInHand;
	void DrawTaskCard();
	Object* playerHand;
private:
	static PlayerManager* instance;
};

