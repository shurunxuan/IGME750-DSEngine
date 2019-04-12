struct VertexToPixel
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D renderTexture0 : register(t2);
SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);

#define KERNEL_SIZE 11

static const float kernel[KERNEL_SIZE] =
{
	0.084264, 0.088139, 0.091276, 0.093585, 0.094998, 0.095474, 0.094998, 0.093585, 0.091276, 0.088139, 0.084264
};

cbuffer GaussianBlurConstantBuffer : register(b0)
{
	float2 textureDimensions; // The render target width/height.
};

struct Output
{
	float4 target0 : SV_TARGET0;
};

Output main(VertexToPixel input)
{
	Output output;
	float4 color = float4(0, 0, 0, 0);
	float h = 1.0f / textureDimensions.y;
	int width = KERNEL_SIZE;
	int center = width / 2;
	for (int y = -center; y <= center; y++)
	{
		//float2 p = float2((x - center) * w, (y - center) * h);
		//float4 c = tex2D(input, saturate(uv.xy + p));
		float4 c = renderTexture0.Sample(linearSampler, saturate(input.uv + float2(0, y * h)));
		float g = kernel[y + center];
		color += c * g;
	}
	output.target0 = float4(color.xyz, 1.0f);
	return output;
}