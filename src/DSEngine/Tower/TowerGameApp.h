#pragma once

#include "DSEngineApp.h"
#include "TaskDeck.h"
#include "BrickDeck.h"
#include "GameManager.h"
#include "PlayerManager.h"


class TowerGameApp :
	public DSEngineApp
{
public:

	~TowerGameApp();

	/**
	 * @brief Override the base class Init function
	 * Should initialize game-specific things
	 */
	void Init() override;

	TaskDeck taskDeck;
	BrickDeck brickDeck;
	GameManager gameManager;
	PlayerManager playerManager;
	BoardManager boardManager;

private:
	SimplePixelShader* unlitShader;

	void CreateScene();
	
};

