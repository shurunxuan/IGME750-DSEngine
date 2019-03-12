#pragma once
#pragma warning(disable:4251)
#include "DSFLogging.h"
#include "SimpleShader.hpp"

class Material
{
public:
	Material(ID3D11Device* d);
	Material(SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d);
	virtual ~Material();

	// Getters
	SimpleVertexShader* GetVertexShaderPtr() const;
	SimplePixelShader* GetPixelShaderPtr() const;
	// Setters
	void SetVertexShaderPtr(SimpleVertexShader* vShader);
	void SetPixelShaderPtr(SimplePixelShader* pShader);

	ID3D11SamplerState* GetSamplerState();

	void InitializeSampler();

	virtual size_t GetMaterialStruct(void** mtlPtr) = 0;

	// texture
	ID3D11ShaderResourceView* diffuseSrvPtr;
	ID3D11ShaderResourceView* normalSrvPtr;

private:
	// Wrappers for DirectX shaders to provide simplified functionality
	// Material class doesn't maintain the life cycle of shader pointers
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	ID3D11Device* device;

	D3D11_SAMPLER_DESC samplerDesc;
	ID3D11SamplerState* samplerState;
};

inline Material::Material(ID3D11Device* d = nullptr)
{
	vertexShader = nullptr;
	pixelShader = nullptr;

	diffuseSrvPtr = nullptr;
	normalSrvPtr = nullptr;
	samplerState = nullptr;

	device = d;
	LOG_INFO << "Material created at <0x" << this << "> by " << __FUNCTION__ << ".";
}


inline Material::Material(SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d)
{
	vertexShader = vtxShader;
	pixelShader = pxlShader;

	diffuseSrvPtr = nullptr;
	normalSrvPtr = nullptr;
	samplerState = nullptr;

	device = d;
	LOG_INFO << "Material created at <0x" << this << "> by " << __FUNCTION__ << ".";
}

inline Material::~Material()
{
	if (samplerState) { samplerState->Release(); }
	if (diffuseSrvPtr) { diffuseSrvPtr->Release(); }
	if (normalSrvPtr) { normalSrvPtr->Release(); }

	LOG_INFO << "Material destroyed at <0x" << this << ">.";
}

inline SimpleVertexShader* Material::GetVertexShaderPtr() const
{
	return vertexShader;
}

inline SimplePixelShader* Material::GetPixelShaderPtr() const
{
	return pixelShader;
}

inline void Material::SetVertexShaderPtr(SimpleVertexShader* vShader)
{
	vertexShader = vShader;
}

inline void Material::SetPixelShaderPtr(SimplePixelShader* pShader)
{
	pixelShader = pShader;
}

inline ID3D11SamplerState* Material::GetSamplerState()
{
	return samplerState;
}

inline void Material::InitializeSampler()
{
	if (samplerState) return;

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = device->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(hr))
		LOG_ERROR << "CreateSamplerState failed at <0x" << this << ">.";
}
