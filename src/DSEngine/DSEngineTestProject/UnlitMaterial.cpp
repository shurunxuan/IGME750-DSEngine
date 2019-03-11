#include "UnlitMaterial.h"

size_t UnlitMaterial::GetMaterialStruct(void** mtlPtr)
{
	*mtlPtr = &parameters;
	return sizeof(UnlitMaterialStruct);
}
