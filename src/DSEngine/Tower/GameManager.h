#pragma once

#include "Brick.h"
#include "BoardManager.h"

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
	

public:
	static GameManager* getInstance();
	GameState currentGameState;	
	void Update(GameState);
	void CheckTaskCompletion();
	Brick* selectedBrick;
	bool ContinousDrawTask = true;
	GameManager();
	~GameManager();
};


