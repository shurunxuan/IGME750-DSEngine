#define MAX_LIGHTS 128
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position				: SV_POSITION;	// XYZW position (System Value Position)
	float4 worldPos				: POSITION0;
	float3 normal				: NORMAL;
	float2 uv					: TEXCOORD;
	float3 tangent				: TANGENT;
	//float4 lViewSpacePos		: POSITION1;
};


struct Light
{
	int Type;
	float3 Direction;// 16 bytes
	float Range;
	float3 Position;// 32 bytes
	float Intensity;
	float3 Color;// 48 bytes
	float SpotFalloff;
	float3 AmbientColor;// 64 bytes
};
struct Material
{
	float4 color;
};

cbuffer materialData : register(b0)
{
	Material material;
}

cbuffer cameraData : register(b2)
{
	float3 CameraPosition;
};

cbuffer lightData : register(b2)
{
	Light lights[MAX_LIGHTS];
	int lightCount;
};

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	return material.color;
}