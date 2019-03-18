cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VertexShaderInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 direction	: POSITION;
};

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;
	output.direction = input.position;
	// Notice: this world matrix is camera translation, no rotation / scale
	matrix worldViewProj = mul(mul(world, view), projection);

	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.position.z = output.position.w;

	return output;
}