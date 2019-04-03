#include "GameManager.h"
#include "TaskDeck.h"
#include "PlayerManager.h"
GameManager* GameManager::instance = 0;

GameManager* GameManager::getInstance()
{
	return instance;
}

GameManager::GameManager()
{
	instance = this;
	Update(Initial);
}


GameManager::~GameManager()
{
}

void GameManager::Update(GameState gs)
{
	switch (gs)
	{
	case Initial:		
		TaskDeck::getInstance()->InitDeck();
		break;
	case Idle:
		break;
	case BrickSelected:
		break;
	case DrawBrick:
		break;
	case DrawTask:
		if (ContinousDrawTask == true)
		{
			PlayerManager::getInstance()->DrawTaskCard();
		}
		break;
	case End:
		break;
	default:
		break;
	}
}