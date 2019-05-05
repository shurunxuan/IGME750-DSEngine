#pragma once

#include "DSEngineApp.h"
#include "TaskDeck.h"
#include "BrickDeck.h"
#include "GameManager.h"
#include "PlayerManager.h"
#include "PPGaussianBlurMaterial.h"
#include "PPDarkCornerMaterial.h"
#include "SSAOMaterial.h"


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
	SimplePixelShader* refractionShader = nullptr;
	SimpleVertexShader* refractionVShader = nullptr;

	void CreateScene();

	// Post-Processing
	SimplePixelShader* ppDarkCornerPS = nullptr;
	SimplePixelShader* ppGaussianBlurUPS = nullptr;
	SimplePixelShader* ppGaussianBlurVPS = nullptr;
	SimplePixelShader* ppAddPS = nullptr;
	SimplePixelShader* ppMultiplyPS = nullptr;
	SimplePixelShader* ppSSAOPS = nullptr;
	SimpleVertexShader* ppSSAOVS = nullptr;
	PPDarkCornerMaterial* darkCornerMaterial = nullptr;
	PPGaussianBlurMaterial* blurUMaterial = nullptr;
	PPGaussianBlurMaterial* blurVMaterial = nullptr;
	PPGaussianBlurMaterial* blurSSAOUMaterial = nullptr;
	PPGaussianBlurMaterial* blurSSAOVMaterial = nullptr;
	PostProcessingMaterial* applyBloomMaterial = nullptr;
	PostProcessingMaterial* applySSAOMaterial = nullptr;
	SSAOMaterial* ssaoMaterial = nullptr;
};

