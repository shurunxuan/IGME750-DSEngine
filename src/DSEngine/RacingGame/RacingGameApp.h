#pragma once
#include "DSEngineApp.h"

class RacingGameApp :
	public DSEngineApp
{
public:

	~RacingGameApp();

	/**
	 * @brief Override the base class Init function
	 * Should initialize game-specific things
	 */
	void Init() override;

private:
};

