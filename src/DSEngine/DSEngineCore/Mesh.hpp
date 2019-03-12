#pragma once
#pragma warning(disable:4251)
#include <DirectXMath.h>

#include <string>
#include <d3d11.h>
#include <vector>
#include "DSFLogging.h"

// --------------------------------------------------------
// A custom vertex definition
//
// You will eventually ADD TO this, and/or make more of these!
// --------------------------------------------------------
struct Vertex
{
	DirectX::XMFLOAT3 Position;	    // The position of the vertex
	DirectX::XMFLOAT3 Normal;       // The normal of the vertex
	DirectX::XMFLOAT2 UV;			// The texture uv of the vertex
	DirectX::XMFLOAT3 Tangent;
};


class Mesh
{
public:
	Mesh(Vertex* vertices, int verticesCount, int* indices, int indicesCount, ID3D11Device* device);
	~Mesh();

	// Getters
	ID3D11Buffer* GetVertexBuffer() const { return vertexBuffer; }
	ID3D11Buffer* GetIndexBuffer() const { return indexBuffer; }
	int GetIndexCount() const { return indexCount; }

	// Bounding Box
	DirectX::XMFLOAT3 BoundingBoxCenter;
	DirectX::XMFLOAT3 BoundingBoxExtents;

private:
	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	int indexCount;
};

inline Mesh::Mesh(Vertex* vertices, int verticesCount, int* indices, int indicesCount, ID3D11Device* device)
{
	indexCount = indicesCount;

	// Create the VERTEX BUFFER description -----------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * verticesCount;       // 3 = number of vertices in the buffer
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
		LOG_ERROR << "Error when creating vertex buffer: ID3D11Device is null.";
		system("pause");
		exit(-1);
	}

	// Create the INDEX BUFFER description ------------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * indexCount;         // 3 = number of indices in the buffer
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

	// Calculate Bounding Box out of vertices
	DirectX::XMFLOAT3 lower = { FLT_MAX , FLT_MAX , FLT_MAX };
	DirectX::XMFLOAT3 upper = { -FLT_MAX , -FLT_MAX , -FLT_MAX };

	for (int i = 0; i < verticesCount; ++i)
	{
		if (lower.x > vertices[i].Position.x)
			lower.x = vertices[i].Position.x;
		if (lower.y > vertices[i].Position.y)
			lower.y = vertices[i].Position.y;
		if (lower.z > vertices[i].Position.z)
			lower.z = vertices[i].Position.z;
		if (upper.x < vertices[i].Position.x)
			upper.x = vertices[i].Position.x;
		if (upper.y < vertices[i].Position.y)
			upper.y = vertices[i].Position.y;
		if (upper.z < vertices[i].Position.z)
			upper.z = vertices[i].Position.z;
	}

	const DirectX::XMFLOAT3 half((upper.x - lower.x) * 0.5f,
		(upper.y - lower.y) * 0.5f,
		(upper.z - lower.z) * 0.5f);

	BoundingBoxCenter.x = lower.x + half.x;
	BoundingBoxCenter.y = lower.y + half.y;
	BoundingBoxCenter.z = lower.z + half.z;

	BoundingBoxExtents = half;

	LOG_INFO << "Mesh created at <0x" << this << "> by " << __FUNCTION__ << ".";
}


inline Mesh::~Mesh()
{
	if (vertexBuffer) { vertexBuffer->Release(); }
	if (indexBuffer) { indexBuffer->Release(); }

	LOG_INFO << "Mesh destroyed at <0x" << this << ">.";
}