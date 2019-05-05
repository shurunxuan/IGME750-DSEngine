#pragma once
#include "Object.hpp"
#include "PBRMaterial.hpp"

class RefractionMaterial :
	public PBRMaterial
{
public:
	explicit RefractionMaterial(ID3D11Device* d, ID3D11DeviceContext* c);
	RefractionMaterial(SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d, ID3D11DeviceContext* c);
	~RefractionMaterial();

	void SetShaderData() override;
private:
	ID3D11SamplerState* clampSampler;
};
