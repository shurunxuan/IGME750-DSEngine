#pragma once

#include <string>
#include <d3d11.h>
#include <codecvt>
#include <DirectXTK/DDSTextureLoader.h>
#include "Mesh.hpp"
#include "SimpleShader.hpp"

class Skybox
{
public:
	Skybox(ID3D11Device* d, ID3D11DeviceContext* c, const std::wstring& cubeMapFile, const std::wstring& irradianceMapFile);
	~Skybox();

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;

	SimpleVertexShader* GetVertexShader() const;
	SimplePixelShader* GetPixelShader() const;

	ID3D11SamplerState* GetSamplerState() const;

	ID3D11ShaderResourceView* GetCubeMapSRV() const;
	ID3D11ShaderResourceView* GetIrradianceMapSRV() const;
private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	D3D11_SAMPLER_DESC samplerDesc;
	ID3D11SamplerState* samplerState;

	ID3D11ShaderResourceView* cubeMapSrv;

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
