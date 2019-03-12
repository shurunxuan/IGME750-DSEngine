#include "PBRMaterial.h"

size_t PBRMaterial::GetMaterialStruct(void** mtlPtr)
{
	*mtlPtr = &parameters;
	return sizeof(PBRMaterialStruct);
}
