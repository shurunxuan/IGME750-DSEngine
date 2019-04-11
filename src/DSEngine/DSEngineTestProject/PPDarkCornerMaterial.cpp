#include "PPDarkCornerMaterial.h"


PPDarkCornerMaterial::PPDarkCornerMaterial(int sourceCount, std::vector<int> sourceIndices, int targetCount, std::vector<int> targetIndices, ID3D11Device* d)
	: PostProcessingMaterial(sourceCount, sourceIndices, targetCount, targetIndices, d)
{
}

PPDarkCornerMaterial::PPDarkCornerMaterial(int sourceCount, std::vector<int> sourceIndices, int targetCount, std::vector<int> targetIndices, SimpleVertexShader* vtxShader,
	SimplePixelShader* pxlShader, ID3D11Device* d)
	: PostProcessingMaterial(sourceCount, sourceIndices, targetCount, targetIndices, vtxShader, pxlShader, d)
{
}

PPDarkCornerMaterial::~PPDarkCornerMaterial()
{
}

void PPDarkCornerMaterial::SetShaderData()
{
}

size_t PPDarkCornerMaterial::GetMaterialStruct(void** mtlPtr)
{
	*mtlPtr = &parameters;
	return sizeof(DarkCornerParameters);
}
