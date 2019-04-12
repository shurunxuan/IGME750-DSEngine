struct VertexToPixel
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D renderTexture0 : register(t2); // Depth
Texture2D renderTexture1 : register(t3); // Normals
SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);
SamplerComparisonState pointComparisonSampler : register(s2);
SamplerComparisonState linearComparisonSampler : register(s3);

struct Output
{
	float4 target0 : SV_TARGET0;
};

float rand(float2 p, float param) { return frac(cos(dot(p, float2(23.14069263277926, 2.665144142690225))) * param); }
float3 rand3D(float2 p)
{
	return float3(
		rand(p, 12345.6789f),
		rand(p, 98765.4321f),
		rand(p, 44444.5555f));
}

Output main(VertexToPixel input)
{
	Output output;

	const float total_strength = 1.0f;
	const float base = 0.2f;

	const float area = 0.0075f;
	const float falloff = 0.000001f;

	const float radius = 0.0002f;

	const int samples = 16;

	float3 sample_sphere[samples] = {
		float3(0.5381, 0.1856,-0.4319), float3(0.1379, 0.2486, 0.4430),
		float3(0.3371, 0.5679,-0.0057), float3(-0.6999,-0.0451,-0.0019),
		float3(0.0689,-0.1598,-0.8547), float3(0.0560, 0.0069,-0.1843),
		float3(-0.0146, 0.1402, 0.0762), float3(0.0100,-0.1924,-0.0344),
		float3(-0.3577,-0.5301,-0.4358), float3(-0.3169, 0.1063, 0.0158),
		float3(0.0103,-0.5869, 0.0046), float3(-0.0897,-0.4940, 0.3287),
		float3(0.7119,-0.0154,-0.0918), float3(-0.0533, 0.0596,-0.5411),
		float3(0.0352,-0.0631, 0.5460), float3(-0.4776, 0.2847,-0.0271)
	};

	float3 random = normalize(rand3D(input.uv));

	float depth = renderTexture0.Sample(pointSampler, input.uv).r;

	float3 position = float3(input.uv, depth);
	float3 normal = -renderTexture1.Sample(pointSampler, input.uv).rgb;

	if (length(normal) < 0.1f)
	{
		output.target0 = float4(1.0f, 1.0f, 1.0f, 1.0f);
		return output;
	}

	float radius_depth = radius / depth;

	float occlusion = 0.0f;

	[unroll] for (int i = 0; i < samples; ++i)
	{
		float3 ray = radius_depth * reflect(sample_sphere[i], random);
		float3 hemi_ray = position + sign(dot(ray, normal)) * ray;

		float occ_depth = renderTexture0.Sample(linearSampler, saturate(hemi_ray.xy)).r;
		float difference = depth - occ_depth;

		occlusion += step(falloff, difference) * (1.0f - smoothstep(falloff, area, difference));
	}

	float ao = 1.0f - total_strength * occlusion * (1.0f / samples);
	output.target0 = float4(saturate(ao + base).rrr, 1.0f);

	return output;
}