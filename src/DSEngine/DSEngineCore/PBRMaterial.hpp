#pragma once

#include "Material.hpp"
#include <locale>
#include <codecvt>
#include <string>
#include <d3d11.h>
#include <DirectXTK/WICTextureLoader.h>
#include "DSFLogging.h"

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

	bool LoadDiffuseTexture(const std::wstring& diffuseTexture);
	bool LoadNormalTexture(const std::wstring& normalTexture);

	bool LoadDiffuseTexture(const std::string& diffuseTexture);
	bool LoadNormalTexture(const std::string& normalTexture);

private:
	ID3D11ShaderResourceView* diffuseSrvPtr;
	ID3D11ShaderResourceView* normalSrvPtr;

	D3D11_SAMPLER_DESC samplerDesc;
	ID3D11SamplerState* samplerState;

	ID3D11DeviceContext* context;
};

inline PBRMaterial::PBRMaterial(ID3D11Device* d, ID3D11DeviceContext* c)
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

inline PBRMaterial::PBRMaterial(SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d, ID3D11DeviceContext* c)
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

inline PBRMaterial::~PBRMaterial()
{
	if (samplerState) { samplerState->Release(); }
	if (diffuseSrvPtr) { diffuseSrvPtr->Release(); }
	if (normalSrvPtr) { normalSrvPtr->Release(); }
}

inline size_t PBRMaterial::GetMaterialStruct(void** mtlPtr)
{
	*mtlPtr = &parameters;
	return sizeof(PBRMaterialStruct);
}

inline void PBRMaterial::SetShaderData()
{
	// Sampler and Texture
	pixelShader->SetSamplerState("basicSampler", samplerState);
	pixelShader->SetShaderResourceView("diffuseTexture", diffuseSrvPtr);
	pixelShader->SetShaderResourceView("normalTexture", normalSrvPtr);
}

inline bool PBRMaterial::LoadDiffuseTexture(const std::wstring& diffuseTexture)
{
	if (diffuseSrvPtr) { diffuseSrvPtr->Release(); }
	const HRESULT hr = DirectX::CreateWICTextureFromFileEx(device, context, diffuseTexture.c_str(), D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
		0, 0, DirectX::WIC_LOADER_FORCE_SRGB, nullptr, &diffuseSrvPtr);
	if (SUCCEEDED(hr))
		if (diffuseSrvPtr)
		{
			parameters.hasDiffuseTexture = 1;
			LOG_INFO << "Loaded Diffuse Texture \"" << diffuseTexture << "\"";
		}
	return SUCCEEDED(hr);
}

inline bool PBRMaterial::LoadNormalTexture(const std::wstring& normalTexture)
{
	if (normalSrvPtr) { normalSrvPtr->Release(); }
	const HRESULT hr = DirectX::CreateWICTextureFromFile(device, context, normalTexture.c_str(), nullptr, &normalSrvPtr);
	if (SUCCEEDED(hr))
		if (normalSrvPtr)
		{
			parameters.hasNormalMap = 1;
			LOG_INFO << "Loaded Normal Map \"" << normalTexture << "\"";
		}
	return SUCCEEDED(hr);
}

inline bool PBRMaterial::LoadDiffuseTexture(const std::string& diffuseTexture)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter; 
	std::wstring wide = converter.from_bytes(diffuseTexture);
	return LoadDiffuseTexture(wide);
}

inline bool PBRMaterial::LoadNormalTexture(const std::string& normalTexture)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wide = converter.from_bytes(normalTexture);
	return LoadNormalTexture(wide);
}
