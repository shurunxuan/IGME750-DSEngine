#include "PPGaussianBlurMaterial.h"

PPGaussianBlurMaterial::PPGaussianBlurMaterial(int sourceCount, std::vector<int> sourceIndices, int targetCount,
	std::vector<int> targetIndices, ID3D11Device* d)
	: PostProcessingMaterial(sourceCount, sourceIndices, targetCount, targetIndices, d)
{
}

PPGaussianBlurMaterial::PPGaussianBlurMaterial(int sourceCount, std::vector<int> sourceIndices, int targetCount,
	std::vector<int> targetIndices, SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d)
	: PostProcessingMaterial(sourceCount, sourceIndices, targetCount, targetIndices, vtxShader, pxlShader, d)
{
}

PPGaussianBlurMaterial::~PPGaussianBlurMaterial()
{
}

void PPGaussianBlurMaterial::SetShaderData()
{
	const DirectX::XMFLOAT2 size { float(*width), float(*height) };
	pixelShader->SetFloat2("textureDimensions", size);
}

size_t PPGaussianBlurMaterial::GetMaterialStruct(void** mtlPtr)
{
	*mtlPtr = nullptr;
	return 0;
}

void PPGaussianBlurMaterial::SetScreenSizePtr(int* width, int* height)
{
	this->width = width;
	this->height = height;
}
