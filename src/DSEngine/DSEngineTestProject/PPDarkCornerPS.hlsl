struct VertexToPixel
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D renderTexture0 : register(t2);
SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);

struct Material
{
	// Material Data
	float intensity;
	float3 padding;
};

cbuffer materialData : register(b1)
{
	Material material;
};

struct Output
{
	float4 target0 : SV_TARGET0;
};

Output main(VertexToPixel input)
{
	Output output;
	float dis = distance(input.uv, float2(0.5f, 0.5f)) / 0.707f;
	dis = dis * dis;
	dis = dis * material.intensity;

	output.target0 = saturate(renderTexture0.Sample(pointSampler, input.uv) * float4(1 - dis, 1 - dis, 1 - dis, 1));

	return output;
}