/**
 * @file TestGameApp.h
 * @author Victor Shu
 * @brief This file contains the example/test class for 
 * the game that is using DSEngine.
 * 
 * @version 0.1
 * @date 2019/02/20
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#pragma once
#include "DSEngineApp.h"
#include "PPDarkCornerMaterial.h"
#include "PPGaussianBlurMaterial.h"
#include "SSAOMaterial.h"
/**
 * @class TestGameApp
 * 
 * @brief Derived from DSEngineApp
 * 
 * @todo This class should contain actual game logic
 * 
 */
class TestGameApp :
	public DSEngineApp
{
public:

	~TestGameApp();

	/**
	 * @brief Override the base class Init function
	 * Should initialize game-specific things
	 */
	void Init() override;

private:
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

