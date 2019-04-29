struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 direction	: POSITION;
};

struct PixelOutput
{
	float4 Target0 : SV_TARGET0;
	float4 Target1 : SV_TARGET1;
	float4 Target2 : SV_TARGET2;
	float4 Target3 : SV_TARGET3;
	float4 Target4 : SV_TARGET4;
	float4 Target5 : SV_TARGET5;
	float4 Target6 : SV_TARGET6;
	float4 Target7 : SV_TARGET7;
};

TextureCube cubemapTexture  : register(t0);
SamplerState basicSampler : register(s0);

PixelOutput main(VertexToPixel input)
{
	PixelOutput output;
	float4 sampleColor = cubemapTexture.Sample(basicSampler, input.direction);
	output.Target0 = sampleColor;
	output.Target1 = saturate(sampleColor - float4(1.0f, 1.0f, 1.0f, 0.0f));
	output.Target2 = float4(0, 0, 0, 1);
	output.Target3 = float4(0, 0, 0, 1);
	output.Target4 = float4(0, 0, 0, 1);
	output.Target5 = float4(0, 0, 0, 1);
	output.Target6 = float4(0, 0, 0, 1);
	output.Target7 = float4(0, 0, 0, 1);
	return output;
}