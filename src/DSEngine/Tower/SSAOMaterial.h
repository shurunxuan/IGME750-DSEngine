#pragma once
#include "PostProcessingMaterial.hpp"
#include "Camera.hpp"
#include <random>

class SSAOMaterial :
	public PostProcessingMaterial
{
public:
	SSAOMaterial(int sourceCount, std::vector<int> sourceIndices, int targetCount, std::vector<int> targetIndices, ID3D11Device* d);
	SSAOMaterial(int sourceCount, std::vector<int> sourceIndices, int targetCount, std::vector<int> targetIndices, SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d);
	~SSAOMaterial();

	void SetCamera(Camera* camera);
	struct SSAOParameters
	{
		float OcclusionRadius;
		float OcclusionFadeStart;
		float OcclusionFadeEnd;
		float SurfaceEpsilon;
	} ssaoCB;

	void SetShaderData() override;
	size_t GetMaterialStruct(void** mtlPtr) override;
private:
	Camera* camera;
	std::array<DirectX::XMFLOAT4, 14> mOffsets;

	std::default_random_engine e;
	std::uniform_real_distribution<> dis;

	ID3D11ShaderResourceView* randomVectorSRV;

	void BuildOffsetVectors();
};


