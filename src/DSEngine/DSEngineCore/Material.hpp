/**
 * @file Material.hpp
 * @author Victor Shu
 * @brief This file contains the class that represents
 * the material of a MeshRenderer
 *
 * @version 0.1
 * @date 2019/03/11
 *
 * @copyright Copyright (c) 2019
 *
 */

#pragma once
#pragma warning(disable:4251)
#include "SimpleShader.hpp"
#include "DSFLogging.h"

 /**
  * @brief The material of a MeshRenderer
  *
  */
class Material
{
public:
	friend class DSSRendering;
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

	/**
	 * Set the blend mode of the material
	 *
	 * Have no effect when transparent is false (Set the transparent variable first!)
	 */
	void SetBlendMode(D3D11_RENDER_TARGET_BLEND_DESC blendDesc);

	/**
	 * @brief Indicates if the material is transparent
	 *
	 * This will have effect on the order of the mesh being rendered
	 * Set this first before setting the blend mode
	 */
	bool transparent;

	/**
	 * @brief Describe whether the material needs the buffer before it's drawn
	 * 
	 * The order can't be promised unless its transparent
	 */
	bool grab;

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
	/**
	 * @brief Indicates the blend mode
	 *
	 * Will be ignored if the transparent is false
	 */
	ID3D11BlendState* blendState;
};

inline Material::Material(ID3D11Device* d = nullptr)
{
	vertexShader = nullptr;
	pixelShader = nullptr;
	blendState = nullptr;
	transparent = false;
	grab = false;

	device = d;
}


inline Material::Material(SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d)
{
	vertexShader = vtxShader;
	pixelShader = pxlShader;
	blendState = nullptr;
	transparent = false;
	grab = false;

	device = d;
}

inline Material::~Material()
{
	if (blendState != nullptr)
		blendState->Release();
}

inline SimpleVertexShader* Material::GetVertexShaderPtr() const
{
	return vertexShader;
}

inline SimplePixelShader* Material::GetPixelShaderPtr() const
{
	return pixelShader;
}

inline void Material::SetVertexShaderPtr(SimpleVertexShader * vShader)
{
	vertexShader = vShader;
}

inline void Material::SetPixelShaderPtr(SimplePixelShader * pShader)
{
	pixelShader = pShader;
}

inline void Material::SetMaterialData()
{
	void* materialData = nullptr;
	const size_t materialSize = GetMaterialStruct(&materialData);
	// Material Data
	if (materialData != nullptr)
		pixelShader->SetData(
			"material",
			materialData,
			int(materialSize)
		);

	SetShaderData();
}

inline void Material::SetBlendMode(D3D11_RENDER_TARGET_BLEND_DESC blendDesc)
{
	if (!transparent)
	{
		LOG_WARNING << "Setting blend mode on a non-transparent material will have no effect!";
		return;
	}
	D3D11_BLEND_DESC blendStateDesc;
	blendStateDesc.AlphaToCoverageEnable = false;
	blendStateDesc.IndependentBlendEnable = false;
	for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		blendStateDesc.RenderTarget[i] = blendDesc;
	device->CreateBlendState(&blendStateDesc, &blendState);
}

///@endcond
