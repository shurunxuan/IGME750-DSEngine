#include "BoardManager.h"
BoardManager* BoardManager::instance = 0;

BoardManager* BoardManager::getInstance()
{
	return instance;
}

BoardManager::BoardManager()
{
	instance = this;
}


BoardManager::~BoardManager()
{
}
