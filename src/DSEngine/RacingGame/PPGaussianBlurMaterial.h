#pragma once
#include "PostProcessingMaterial.hpp"

class PPGaussianBlurMaterial :
	public PostProcessingMaterial
{
public:
	PPGaussianBlurMaterial(int sourceCount, std::vector<int> sourceIndices, int targetCount, std::vector<int> targetIndices, ID3D11Device* d);
	PPGaussianBlurMaterial(int sourceCount, std::vector<int> sourceIndices, int targetCount, std::vector<int> targetIndices, SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d);
	~PPGaussianBlurMaterial();

	void SetShaderData() override;
	size_t GetMaterialStruct(void** mtlPtr) override;

	void SetScreenSizePtr(int* width, int* height);

private:
	int* width;
	int* height;
};

