#pragma once
#include "PostProcessingMaterial.hpp"

class PPDarkCornerMaterial :
	public PostProcessingMaterial
{
public:
	PPDarkCornerMaterial(int sourceCount, std::vector<int> sourceIndices, int targetCount, std::vector<int> targetIndices, ID3D11Device* d);
	PPDarkCornerMaterial(int sourceCount, std::vector<int> sourceIndices, int targetCount, std::vector<int> targetIndices, SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d);
	~PPDarkCornerMaterial();

	struct DarkCornerParameters
	{
		float intensity;
		DirectX::XMFLOAT3 padding;
	} parameters;

	void SetShaderData() override;
	size_t GetMaterialStruct(void** mtlPtr) override;
};

