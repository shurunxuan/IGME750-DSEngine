struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 direction	: POSITION;
};

TextureCube cubemapTexture  : register(t2);
SamplerState basicSampler : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 sampleColor = cubemapTexture.Sample(basicSampler, input.direction);

	return sampleColor;
}