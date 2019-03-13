#pragma once
#pragma warning(disable:4251)
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

	virtual size_t GetMaterialStruct(void** mtlPtr) = 0;
	void SetMaterialData();

protected:
	virtual void SetShaderData() = 0;

	ID3D11Device* device;
	// Wrappers for DirectX shaders to provide simplified functionality
	// Material class doesn't maintain the life cycle of shader pointers
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
};

inline Material::Material(ID3D11Device* d = nullptr)
{
	vertexShader = nullptr;
	pixelShader = nullptr;

	device = d;
}


inline Material::Material(SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d)
{
	vertexShader = vtxShader;
	pixelShader = pxlShader;

	device = d;
}

inline Material::~Material()
{

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

inline void Material::SetMaterialData()
{
	void* materialData;
	const size_t materialSize = GetMaterialStruct(&materialData);
	// Material Data
	pixelShader->SetData(
		"material",
		materialData,
		int(materialSize)
	);

	SetShaderData();
}
