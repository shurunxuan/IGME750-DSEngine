struct VertexToPixel
{
	float4 position				: SV_POSITION;
	float4 worldPos				: POSITION0;
	float3 normal				: NORMAL;
	float2 uv					: TEXCOORD;
	float3 tangent				: TANGENT0;
	float4 lViewSpacePos		: POSITION1;
};

struct Material
{
	// Material Data
	float4 color;
};

cbuffer materialData : register(b1)
{
	Material material;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	return material.color;
}