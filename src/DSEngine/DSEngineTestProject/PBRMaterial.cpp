#include "PBRMaterial.h"
#include <DirectXTK/WICTextureLoader.h>

PBRMaterial::PBRMaterial(ID3D11Device* d, ID3D11DeviceContext* c)
	: Material(d)
{
	context = c;

	diffuseSrvPtr = nullptr;
	normalSrvPtr = nullptr;

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = device->CreateSamplerState(&samplerDesc, &samplerState);
}

PBRMaterial::PBRMaterial(SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d, ID3D11DeviceContext* c)
	: Material(vtxShader, pxlShader, d)
{
	context = c;

	diffuseSrvPtr = nullptr;
	normalSrvPtr = nullptr;

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = device->CreateSamplerState(&samplerDesc, &samplerState);
}

PBRMaterial::~PBRMaterial()
{
	if (samplerState) { samplerState->Release(); }
	if (diffuseSrvPtr) { diffuseSrvPtr->Release(); }
	if (normalSrvPtr) { normalSrvPtr->Release(); }
}

size_t PBRMaterial::GetMaterialStruct(void** mtlPtr)
{
	*mtlPtr = &parameters;
	return sizeof(PBRMaterialStruct);
}

void PBRMaterial::SetShaderData()
{
	// Sampler and Texture
	pixelShader->SetSamplerState("basicSampler", samplerState);
	pixelShader->SetShaderResourceView("diffuseTexture", diffuseSrvPtr);
	pixelShader->SetShaderResourceView("normalTexture", normalSrvPtr);
}

void PBRMaterial::LoadDiffuseTexture(std::wstring diffuseTexture)
{
	if (diffuseSrvPtr) { diffuseSrvPtr->Release(); }
	DirectX::CreateWICTextureFromFileEx(device, context, diffuseTexture.c_str(), D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
		0, 0, DirectX::WIC_LOADER_FORCE_SRGB, nullptr, &diffuseSrvPtr);
	if (diffuseSrvPtr) { parameters.hasDiffuseTexture = 1; }

}

void PBRMaterial::LoadNormalTexture(std::wstring normalTexture)
{
	if (normalSrvPtr) { normalSrvPtr->Release(); }
	DirectX::CreateWICTextureFromFileEx(device, context, normalTexture.c_str(), D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
		0, 0, DirectX::WIC_LOADER_FORCE_SRGB, nullptr, &normalSrvPtr);
	if (normalSrvPtr) { parameters.hasNormalMap = 1; }
}
