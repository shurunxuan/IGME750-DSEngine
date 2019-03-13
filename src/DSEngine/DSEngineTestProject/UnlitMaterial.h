#pragma once
#include "Material.hpp"

class UnlitMaterial :
	public Material
{
public:
	using Material::Material;

	struct UnlitMaterialStruct
	{
		// Some parameters for lighting
		DirectX::XMFLOAT4 color = { 0.0f, 1.0f, 0.0f, 1.0f };
	} parameters;

	size_t GetMaterialStruct(void** mtlPtr) override;

private:
	void SetShaderData() override;
};


