/**
 * @file PostProcessingMaterial.hpp
 * @author Victor Shu
 * @brief This file contains the class that represents
 * the material for post processing
 *
 * @version 0.1
 * @date 2019/04/11
 *
 * @copyright Copyright (c) 2019
 *
 */

#pragma once

#include "Material.hpp"

 /**
  * @brief The default post processing material that does not set any material/texture data
  *
  */
class PostProcessingMaterial :
	public Material
{
public:
	/**
	 * @brief Construct a new PostProcessingMaterial
	 *
	 * @param sourceCount The number of source render targets
	 * @param sourceIndices The source render target indices
	 * @param targetCount The number of target render targets
	 * @param targetIndices The target render target indices, negative value means back buffer (screen)
	 * @param d Direct3D 11 device
	 */
	explicit PostProcessingMaterial(int sourceCount, std::vector<int> sourceIndices, int targetCount, std::vector<int> targetIndices, ID3D11Device* d);
	/**
	 * @brief Construct a new PostProcessingMaterial with shaders
	 *
	 * @param sourceCount The number of source render targets
	 * @param sourceIndices The source render target indices
	 * @param targetCount The number of target render targets
	 * @param targetIndices The target render target indices, negative value means back buffer (screen)
	 * @param vtxShader The vertex shader
	 * @param pxlShader The pixel shader
	 * @param d Direct3D 11 device
	 */
	PostProcessingMaterial(int sourceCount, std::vector<int> sourceIndices, int targetCount, std::vector<int> targetIndices, SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d);
	/**
	 * @brief Destroy the PostProcessingMaterial object
	 *
	 */
	~PostProcessingMaterial();

	/**
	 * @brief Get the source render target indices
	 * 
	 * @param[out] indices The desired index array
	 * 
	 * @return size_t The size of the array
	 */
	size_t GetSourceIndices(int** indices);

	/**
	 * @brief Get the target render target indices
	 *
	 * @param[out] indices The desired index array
	 *
	 * @return size_t The size of the array
	 */
	size_t GetTargetIndices(int** indices);

	/**
	 * @brief Get the Material Struct, which does nothing for a default post processing material
	 *
	 * @param mtlPtr Output the nullptr
	 * @return size_t 0
	 */
	size_t GetMaterialStruct(void** mtlPtr) override;
	/**
	 * @brief Does nothing for a default post processing material
	 *
	 */
	void SetShaderData() override;

protected:
	/**
	 * @brief The source render target index
	 */
	std::vector<int> sourceIndices;
	/**
	 * @brief The target render target index, negative value means back buffer (screen)
	 */
	std::vector<int> targetIndices;

};

inline PostProcessingMaterial::PostProcessingMaterial(int sourceCount, std::vector<int> sourceIndices, int targetCount, std::vector<int> targetIndices, ID3D11Device* d)
	: Material(d)
{
	this->sourceIndices.resize(sourceCount);
	for (int i = 0; i < sourceCount; ++i)
	{
		this->sourceIndices[i] = sourceIndices[i];
	}
	this->targetIndices.resize(targetCount);
	for (int i = 0; i < targetCount; ++i)
	{
		this->targetIndices[i] = targetIndices[i];
	}
}

inline PostProcessingMaterial::PostProcessingMaterial(int sourceCount, std::vector<int> sourceIndices, int targetCount, std::vector<int> targetIndices, SimpleVertexShader* vtxShader,
	SimplePixelShader* pxlShader, ID3D11Device* d)
	: Material(vtxShader, pxlShader, d)
{
	this->sourceIndices.resize(sourceCount);
	for (int i = 0; i < sourceCount; ++i)
	{
		this->sourceIndices[i] = sourceIndices[i];
	}
	this->targetIndices.resize(targetCount);
	for (int i = 0; i < targetCount; ++i)
	{
		this->targetIndices[i] = targetIndices[i];
	}
}

inline PostProcessingMaterial::~PostProcessingMaterial()
= default;

inline size_t PostProcessingMaterial::GetSourceIndices(int** indices)
{
	*indices = &*sourceIndices.begin();
	return sourceIndices.size();
}

inline size_t PostProcessingMaterial::GetTargetIndices(int** indices)
{
	*indices = &*targetIndices.begin();
	return targetIndices.size();
}

inline size_t PostProcessingMaterial::GetMaterialStruct(void** mtlPtr)
{
	*mtlPtr = nullptr;
	return 0;
}

inline void PostProcessingMaterial::SetShaderData()
{
}
