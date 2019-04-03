#pragma once

#include "Brick.h"

enum GameState {
	Initial = 1,
	Idle,
	BrickSelected,
	DrawBrick,
	DrawTask,
	End,
};

class GameManager 
{
private:
	static GameManager* instance;
	bool ContinousDrawTask = true;

public:
	static GameManager* getInstance();
	GameState currentGameState;	
	void Update(GameState);
	//Brick selectedBrick;
	GameManager();
	~GameManager();
};


