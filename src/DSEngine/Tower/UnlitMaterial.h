#pragma once
#include "Material.hpp"

class UnlitMaterial :
	public Material
{
public:
	explicit UnlitMaterial(ID3D11Device* d);
	UnlitMaterial(SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d);
	~UnlitMaterial();

	struct UnlitMaterialStruct
	{
		DirectX::XMFLOAT4 Color;
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
};

