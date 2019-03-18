/**
 * @file Skybox.hpp
 * @author Victor Shu
 * @brief This file contains the class that represents
 * the skybox
 * 
 * @version 0.1
 * @date 2019/03/12
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once
#pragma warning(disable:4251)

#include <string>
#include <d3d11.h>
#include <codecvt>
#include <DirectXTK/DDSTextureLoader.h>
#include "Mesh.hpp"
#include "SimpleShader.hpp"

/**
 * @brief The class represents a skybox
 * 
 */
class Skybox
{
public:
	/**
	 * @brief Construct a new Skybox from cubemap files
	 * 
	 * @param d Direct3D 11 device
	 * @param c Direct3D 11 device context
	 * @param cubeMapFile The file name of the cubemap of the skybox
	 * @param irradianceMapFile The file name of the irradiance map of the cubemap
	 */
	Skybox(ID3D11Device* d, ID3D11DeviceContext* c, const std::wstring& cubeMapFile, const std::wstring& irradianceMapFile);
	/**
	 * @brief Destroy the Skybox object
	 * 
	 */
	~Skybox();

	/**
	 * @brief Get the Vertex Buffer of the skybox
	 * 
	 * @return ID3D11Buffer* The Vertex Buffer
	 */
	ID3D11Buffer* GetVertexBuffer() const;
	/**
	 * @brief Get the Index Buffer of the skybox
	 * 
	 * @return ID3D11Buffer* The Index Buffer
	 */
	ID3D11Buffer* GetIndexBuffer() const;

	/**
	 * @brief Get the Vertex Shader of the skybox
	 * 
	 * @return SimpleVertexShader* The vertex shader
	 */
	SimpleVertexShader* GetVertexShader() const;
	/**
	 * @brief Get the Pixel Shader of the skybox
	 * 
	 * @return SimplePixelShader* The pixel shader
	 */
	SimplePixelShader* GetPixelShader() const;

	/**
	 * @brief Get the Sampler State for sampling the cubemaps
	 * 
	 * @return ID3D11SamplerState* The sampler state
	 */
	ID3D11SamplerState* GetSamplerState() const;

	/**
	 * @brief Get the shader resource view of the cubemap
	 * 
	 * @return ID3D11ShaderResourceView* The shader resource view of the cubemap
	 */
	ID3D11ShaderResourceView* GetCubeMapSRV() const;
	/**
	 * @brief Get the shader resource view of the irradiance map
	 * 
	 * @return ID3D11ShaderResourceView* The shader resource view of the irradiance map
	 */
	ID3D11ShaderResourceView* GetIrradianceMapSRV() const;
private:
	/**
	 * @brief Direct3D 11 device
	 * 
	 */
	ID3D11Device* device;
	/**
	 * @brief Direct3D 11 device context
	 * 
	 */
	ID3D11DeviceContext* context;

	/**
	 * @brief Vertex Buffer
	 * 
	 */
	ID3D11Buffer* vertexBuffer;
	/**
	 * @brief Index Buffer
	 * 
	 */
	ID3D11Buffer* indexBuffer;

	/**
	 * @brief Skybox vertex shader
	 * 
	 */
	SimpleVertexShader* vertexShader;
	/**
	 * @brief Skybox pixel shader
	 * 
	 */
	SimplePixelShader* pixelShader;

	/**
	 * @brief The description of the sampler state
	 * 
	 */
	D3D11_SAMPLER_DESC samplerDesc;
	/**
	 * @brief The sampler state
	 * 
	 */
	ID3D11SamplerState* samplerState;

	/**
	 * @brief The shader resource view of the cubemap
	 * 
	 */
	ID3D11ShaderResourceView* cubeMapSrv;
	/**
	 * @brief The shader resource view of the irradiance map
	 * 
	 */
	ID3D11ShaderResourceView* irradianceMapSrv;
};

inline Skybox::Skybox(ID3D11Device* d, ID3D11DeviceContext* c, const std::wstring& cubeMapFile, const std::wstring& irradianceMapFile)
{
	device = d;
	context = c;
	DirectX::CreateDDSTextureFromFileEx(device, context, cubeMapFile.c_str(), D3D11_REQ_TEXTURECUBE_DIMENSION, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
		0, 0, true, nullptr, &cubeMapSrv);
	DirectX::CreateDDSTextureFromFileEx(device, context, irradianceMapFile.c_str(), D3D11_REQ_TEXTURECUBE_DIMENSION, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE,
		0, 0, true, nullptr, &irradianceMapSrv);

	Vertex vertices[8];

	vertices[0].Position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
	vertices[1].Position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
	vertices[2].Position = DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f);
	vertices[3].Position = DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f);
	vertices[4].Position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
	vertices[5].Position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);
	vertices[6].Position = DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f);
	vertices[7].Position = DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f);

	int indices[36] =
	{
		0, 2, 3, 0, 1, 2,
		0, 4, 1, 4, 5, 1,
		1, 5, 6, 1, 6, 2,
		7, 3, 2, 7, 2, 6,
		0, 3, 4, 4, 3, 7,
		4, 7, 5, 5, 7, 6
	};

	// Create the VERTEX BUFFER description -----------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 8;       // 3 = number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	if (device)
		device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer);
	else
	{
		system("pause");
		exit(-1);
	}

	// Create the INDEX BUFFER description ------------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * 36;         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	device->CreateBuffer(&ibd, &initialIndexData, &indexBuffer);

	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = device->CreateSamplerState(&samplerDesc, &samplerState);

	vertexShader = new SimpleVertexShader(device, context);
	pixelShader = new SimplePixelShader(device, context);

	vertexShader->LoadShaderFile(L"SkyboxVS.cso");
	pixelShader->LoadShaderFile(L"SkyboxPS.cso");
}

inline Skybox::~Skybox()
{
	delete vertexShader;
	delete pixelShader;

	if (vertexBuffer) { vertexBuffer->Release(); }
	if (indexBuffer) { indexBuffer->Release(); }

	if (samplerState) { samplerState->Release(); }

	if (cubeMapSrv) { cubeMapSrv->Release(); }

	if (irradianceMapSrv) { irradianceMapSrv->Release(); }
}

inline ID3D11Buffer* Skybox::GetVertexBuffer() const
{
	return vertexBuffer;
}

inline ID3D11Buffer* Skybox::GetIndexBuffer() const
{
	return indexBuffer;
}

inline SimpleVertexShader* Skybox::GetVertexShader() const
{
	return vertexShader;
}

inline SimplePixelShader* Skybox::GetPixelShader() const
{
	return pixelShader;
}

inline ID3D11SamplerState* Skybox::GetSamplerState() const
{
	return samplerState;
}

inline ID3D11ShaderResourceView* Skybox::GetCubeMapSRV() const
{
	return cubeMapSrv;
}

inline ID3D11ShaderResourceView* Skybox::GetIrradianceMapSRV() const
{
	return irradianceMapSrv;
}
