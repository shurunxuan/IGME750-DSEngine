struct VertexToPixel
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D renderTexture0 : register(t2);
SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);

struct Output
{
	float4 target0 : SV_TARGET0;
};

Output main(VertexToPixel input)
{
	Output output;

	output.target0 = saturate(renderTexture0.Sample(pointSampler, input.uv));

	return output;
}