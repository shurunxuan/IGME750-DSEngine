#pragma once
#include "DSEngineApp.h"

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

private:
};

