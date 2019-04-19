/**
 * @file PBRMaterial.hpp
 * @author Victor Shu
 * @brief This file contains the class that represents
 * the PBR Material
 * 
 * @version 0.1
 * @date 2019/03/13
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once
#pragma warning(disable:4251)

#include "Material.hpp"
#include <locale>
#include <codecvt>
#include <string>
#include <d3d11.h>
#include <DirectXTK/WICTextureLoader.h>
#include "DSFLogging.h"

/**
 * @brief The default physically based rendering material
 * 
 */
class PBRMaterial final :
	public Material
{
public:
	/**
	 * @brief Construct a new PBRMaterial
	 * 
	 * @param d Direct3D 11 device
	 * @param c Direct3D 11 device context
	 */
	explicit PBRMaterial(ID3D11Device* d, ID3D11DeviceContext* c);
	/**
	 * @brief Construct a new PBRMaterial with shaders
	 * 
	 * @param vtxShader The vertex shader
	 * @param pxlShader The pixel shader
	 * @param d Direct3D 11 device
	 * @param c Direct3D 11 device context
	 */
	PBRMaterial(SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d, ID3D11DeviceContext* c);
	/**
	 * @brief Destroy the PBRMaterial object
	 * 
	 */
	~PBRMaterial();

	/**
	 * @brief The data struct of the PBRMaterial
	 * 
	 * @todo Add roughness map and metalness map!
	 */
	struct PBRMaterialStruct
	{
		/**
		 * @brief The albedo color
		 * 
		 */
		DirectX::XMFLOAT3 albedo = { 1.0f, 1.0f, 1.0f };
		/**
		 * @brief Roughness
		 * 
		 */
		float roughness = 0.1f;
		/**
		 * @brief Metalness
		 * 
		 */
		float metalness = 1.0f;
		/**
		 * @brief Transparency
		 * 
		 * Will be ignored if the material is not set as a transparent material
		 */
		float transparency = 0.0f;
		/**
		 * @brief Indicates if the material includes a normal map
		 * 
		 */
		int hasNormalMap = 0;
		/**
		 * @brief Indicates if the material includes a diffuse texture
		 * 
		 */
		int hasDiffuseTexture = 0;
	} parameters;

	/**
	 * @brief Get the Material Struct
	 * 
	 * @param mtlPtr The pointer points to the PBRMaterialStruct
	 * @return size_t The size of the struct
	 */
	size_t GetMaterialStruct(void** mtlPtr) override;
	/**
	 * @brief Set the sampler state, normal map and diffuse texture to the pixel shader
	 * 
	 */
	void SetShaderData() override;

	/**
	 * @brief Load the diffuse texture
	 * 
	 * @param diffuseTexture The filename of the diffuse texture
	 * @return bool Indicates the load succeeded
	 */
	bool LoadDiffuseTexture(const std::wstring& diffuseTexture);
	/**
	 * @brief Load the normal texture
	 * 
	 * @param diffuseTexture The filename of the normal texture
	 * @return bool Indicates the load succeeded
	 */
	bool LoadNormalTexture(const std::wstring& normalTexture);

	/**
	 * @brief Load the diffuse texture
	 * 
	 * @param diffuseTexture The filename of the diffuse texture
	 * @return bool Indicates the load succeeded
	 */
	bool LoadDiffuseTexture(const std::string& diffuseTexture);
	/**
	 * @brief Load the normal texture
	 * 
	 * @param diffuseTexture The filename of the normal texture
	 * @return bool Indicates the load succeeded
	 */
	bool LoadNormalTexture(const std::string& normalTexture);

private:
	/**
	 * @brief The shader resource view of the diffuse texture
	 * 
	 */
	ID3D11ShaderResourceView* diffuseSrvPtr;
	/**
	 * @brief The shader resource view of the normal texture
	 * 
	 */
	ID3D11ShaderResourceView* normalSrvPtr;

	/**
	 * @brief The description of the sampler state
	 * 
	 */
	D3D11_SAMPLER_DESC samplerDesc;
	/**
	 * @brief The sampler state used for sampling the textures
	 * 
	 */
	ID3D11SamplerState* samplerState;

	/**
	 * @brief Direct3D 11 device context
	 * 
	 */
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
