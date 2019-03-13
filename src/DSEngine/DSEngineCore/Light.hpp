#pragma once
#include <DirectXMath.h>

#define LIGHT_TYPE_DIR 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2

struct Light
{
	int Type;
	DirectX::XMFLOAT3 Direction;// 16 bytes
	float Range;
	DirectX::XMFLOAT3 Position;// 32 bytes
	float Intensity;
	DirectX::XMFLOAT3 Color;// 48 bytes
	float SpotFalloff;
	DirectX::XMFLOAT3 AmbientColor;// 64 bytes
};

inline Light DirectionalLight(DirectX::XMFLOAT3 color, DirectX::XMFLOAT3 direction, float intensity, DirectX::XMFLOAT3 ambientColor)
{
	Light result{};
	result.Color = color;
	result.AmbientColor = ambientColor;
	result.Direction = direction;
	result.Intensity = intensity;
	result.Type = LIGHT_TYPE_DIR;
	return result;
}

inline Light PointLight(DirectX::XMFLOAT3 color, DirectX::XMFLOAT3 position, float range, float intensity)
{
	Light result{};
	result.Color = color;
	result.Position = position;
	result.Range = range;
	result.Intensity = intensity;
	result.Type = LIGHT_TYPE_POINT;
	return result;
}

inline Light SpotLight(DirectX::XMFLOAT3 color, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction, float range, float spotFalloff,
	float intensity)
{
	Light result{};
	result.Color = color;
	result.Position = position;
	result.Direction = direction;
	result.Range = range;
	result.SpotFalloff = spotFalloff;
	result.Intensity = intensity;
	result.Type = LIGHT_TYPE_SPOT;
	return result;
}