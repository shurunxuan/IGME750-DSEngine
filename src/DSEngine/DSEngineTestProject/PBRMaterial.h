#pragma once

#include "Material.hpp"

class PBRMaterial :
	public Material
{
public:
	using Material::Material;

	struct PBRMaterialStruct
	{
		// Some parameters for lighting
		DirectX::XMFLOAT4 color = { 0.0f, 1.0f, 0.0f, 1.0f };
	} parameters;

	size_t GetMaterialStruct(void** mtlPtr) override;
};

