#pragma once
#include "DSEngineApp.h"
#include "PPDarkCornerMaterial.h"
#include "PPGaussianBlurMaterial.h"
#include "SSAOMaterial.h"
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

