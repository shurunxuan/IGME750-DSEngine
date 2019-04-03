#include "PlayerManager.h"

PlayerManager* PlayerManager::instance = nullptr;

PlayerManager* PlayerManager::getInstance()
{
	return instance;
}

void PlayerManager::DrawTaskCard()
{
}

PlayerManager::PlayerManager()
{
	instance = this;
}


PlayerManager::~PlayerManager()
{
}
