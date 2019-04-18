struct VertexToPixel
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D renderTexture0 : register(t2);
Texture2D renderTexture1 : register(t3);
SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);

float4 main(VertexToPixel input) : SV_TARGET
{
	return saturate(renderTexture0.Sample(pointSampler, input.uv) * renderTexture1.Sample(pointSampler, input.uv));
}