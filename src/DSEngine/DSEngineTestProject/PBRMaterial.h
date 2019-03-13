#pragma once

#include "Material.hpp"

class PBRMaterial :
	public Material
{
public:
	explicit PBRMaterial(ID3D11Device* d, ID3D11DeviceContext* c);
	PBRMaterial(SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d, ID3D11DeviceContext* c);
	~PBRMaterial();

	struct PBRMaterialStruct
	{
		DirectX::XMFLOAT3 albedo = { 1.0f, 1.0f, 1.0f };
		float roughness = 0.5f;
		float metalness = 0.5f;
		int hasNormalMap = 0;
		int hasDiffuseTexture = 0;
	} parameters;

	size_t GetMaterialStruct(void** mtlPtr) override;
	void SetShaderData() override;

	void LoadDiffuseTexture(std::wstring diffuseTexture);
	void LoadNormalTexture(std::wstring normalTexture);

private:
	ID3D11ShaderResourceView* diffuseSrvPtr;
	ID3D11ShaderResourceView* normalSrvPtr;

	D3D11_SAMPLER_DESC samplerDesc;
	ID3D11SamplerState* samplerState;

	ID3D11DeviceContext* context;
};

