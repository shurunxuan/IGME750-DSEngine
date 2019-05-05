#include "RefractionMaterial.h"
#include "DSEngineApp.h"


RefractionMaterial::RefractionMaterial(ID3D11Device* d, ID3D11DeviceContext* c)
	: PBRMaterial(d, c)
{
	clampSampler = nullptr;

	D3D11_SAMPLER_DESC rSamp = {};
	rSamp.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	rSamp.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	rSamp.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	rSamp.Filter = D3D11_FILTER_ANISOTROPIC;
	rSamp.MaxAnisotropy = 16;
	rSamp.MaxLOD = D3D11_FLOAT32_MAX;

	// Ask DirectX for the actual object
	device->CreateSamplerState(&rSamp, &clampSampler);
}

RefractionMaterial::RefractionMaterial(SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d, ID3D11DeviceContext* c)
	: PBRMaterial(vtxShader, pxlShader, d, c)
{
	clampSampler = nullptr;

	D3D11_SAMPLER_DESC rSamp = {};
	rSamp.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	rSamp.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	rSamp.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	rSamp.Filter = D3D11_FILTER_ANISOTROPIC;
	rSamp.MaxAnisotropy = 16;
	rSamp.MaxLOD = D3D11_FLOAT32_MAX;

	// Ask DirectX for the actual object
	device->CreateSamplerState(&rSamp, &clampSampler);
}


RefractionMaterial::~RefractionMaterial()
{
	SAFE_RELEASE(clampSampler);
}

void RefractionMaterial::SetShaderData()
{
	PBRMaterial::SetShaderData();
	pixelShader->SetSamplerState("refractSampler", clampSampler);

	DirectX::XMFLOAT4X4 viewMatrix; 
	DirectX::XMStoreFloat4x4(&viewMatrix, App->CurrentActiveScene()->mainCamera->GetViewMatrix());
	pixelShader->SetMatrix4x4("view", viewMatrix);
}