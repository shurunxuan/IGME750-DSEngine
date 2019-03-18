struct VertexShaderInput
{
	float3 pos			: POSITION;     // XYZ position
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT;
};

struct VertexToPixel
{
	float4 pos		: SV_POSITION;
};

cbuffer lightData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
}

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;
	float4 pos = float4(input.pos, 1.0f);

	// Transform the vertex position into projected space.
	pos = mul(pos, world);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;

	return output;
}