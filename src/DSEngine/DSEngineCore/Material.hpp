#pragma once
#pragma warning(disable:4251)
#include "SimpleShader.hpp"

/**
 * @brief The material of a MeshRenderer
 * 
 */
class Material
{
public:
	/**
	 * @brief Construct a new Material object
	 * 
	 * @param d Direct3D 11 device
	 */
	Material(ID3D11Device* d);
	/**
	 * @brief Construct a new Material object with shaders
	 * 
	 * @param vtxShader Vertex Shader
	 * @param pxlShader Pixel Shader
	 * @param d Direct3D 11 device
	 */
	Material(SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d);
	/**
	 * @brief Destroy the Material object
	 * 
	 */
	virtual ~Material();

	/**
	 * @brief Get the Vertex Shader Pointer
	 * 
	 * @return SimpleVertexShader* The Vertex Shader Pointer
	 */
	SimpleVertexShader* GetVertexShaderPtr() const;
	/**
	 * @brief Get the Pixel Shader Pointer
	 * 
	 * @return SimplePixelShader* The Pixel Shader Pointer
	 */
	SimplePixelShader* GetPixelShaderPtr() const;
	
	/**
	 * @brief Set the Vertex Shader Pointer
	 * 
	 * @param vShader The Vertex Shader Pointer
	 */
	void SetVertexShaderPtr(SimpleVertexShader* vShader);
	/**
	 * @brief Set the Pixel Shader Pointer
	 * 
	 * @param pShader The Pixel Shader Pointer
	 */
	void SetPixelShaderPtr(SimplePixelShader* pShader);

	/**
	 * @brief Get the Material Struct
	 * 
	 * This must be implemented by a material
	 * 
	 * @param[out] mtlPtr The pointer points to the material struct
	 * @return size_t The size of the struct
	 */
	virtual size_t GetMaterialStruct(void** mtlPtr) = 0;
	/**
	 * @brief Send the material data to the shader
	 * 
	 */
	void SetMaterialData();

protected:
	/**
	 * @brief Set other data needed by the shaders
	 * 
	 * This must be implemented even if there's no other things to set
	 * 
	 */
	virtual void SetShaderData() = 0;

	/**
	 * @brief The Direct3D 11 device
	 * 
	 */
	ID3D11Device* device;
	/**
	 * @brief The vertex shader
	 * 
	 */
	SimpleVertexShader* vertexShader;
	/**
	 * @brief The pixel shader
	 * 
	 */
	SimplePixelShader* pixelShader;
};

/// @cond INLINE_DEFINITION
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
///@endcond