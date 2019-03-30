#include "UnlitMaterial.h"



UnlitMaterial::UnlitMaterial(ID3D11Device* d)
	: Material(d)
{
}

UnlitMaterial::UnlitMaterial(SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d)
	: Material(vtxShader, pxlShader, d)
{
}


UnlitMaterial::~UnlitMaterial()
{
}

size_t UnlitMaterial::GetMaterialStruct(void** mtlPtr)
{
	*mtlPtr = &parameters;
	return sizeof(UnlitMaterialStruct);
}

void UnlitMaterial::SetShaderData()
{
}
