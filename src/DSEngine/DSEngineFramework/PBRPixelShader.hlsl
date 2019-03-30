#define MAX_LIGHTS 128
#define FLT_EPSILON 1.19209290E-07F

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	float4 position				: SV_POSITION;
	float4 worldPos				: POSITION0;
	float3 normal				: NORMAL;
	float2 uv					: TEXCOORD;
	float3 tangent				: TANGENT0;
	float4 lViewSpacePos		: POSITION1;
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
	int castShadow;
	int shadowID;
	float2 padding;
};

struct Material
{
	// Material Data
	float3 albedo;
	float roughness;
	float metalness;
	int hasNormalMap;
	int hasDiffuseTexture;
};

cbuffer lightData : register(b0)
{
	Light lights[MAX_LIGHTS];
	int lightCount;
};

cbuffer materialData : register(b1)
{
	Material material;
};

cbuffer cameraData : register(b2)
{
	float3 CameraPosition;
};

cbuffer shadowData : register(b3)
{
	//float4x4        cascadeProjection;
	float4          cascadeOffset[3];
	float4          cascadeScale[3];
	int             cascadeLevels; // Number of Cascades
	int             visualizeCascades; // 1 is to visualize the cascades in different colors. 0 is to just draw the scene
	int             pcfBlurForLoopStart; // For loop begin value. For a 5x5 Kernal this would be -2.
	int             pcfBlurForLoopEnd; // For loop end value. For a 5x5 kernel this would be 3.

	// For Map based selection scheme, this keeps the pixels inside of the the valid range.
	// When there is no border, these values are 0 and 1 respectivley.
	float           minBorderPadding;
	float           maxBorderPadding;
	float           shadowBias;  // A shadow map offset to deal with self shadow artifacts.  
										   //These artifacts are aggravated by PCF.
	float           shadowPartitionSize;
	float           cascadeBlendArea; // Amount to overlap when blending between cascades.
	float           texelSize;
	float           nativeTexelSizeInX;
	float           paddingForCB3; // Padding variables exist because CBs must be a multiple of 16 bytes.
}

Texture2D diffuseTexture  : register(t0);
Texture2D normalTexture  : register(t1);
TextureCube cubemap : register(t2);
TextureCube irradianceMap : register(t3);
Texture2DArray shadowMap : register(t4);
SamplerState basicSampler : register(s0);
SamplerComparisonState shadowSampler : register(s1);

// https://www.knaldtech.com/docs/doku.php?id=specular_lys
static const int mipOffset = 0;

float RoughnessFromPerceptualRoughness(float perceptualRoughness)
{
	return perceptualRoughness * perceptualRoughness;
}

float PerceptualRoughnessFromRoughness(float roughness)
{
	return sqrt(max(0.0, roughness));
}

float SpecularPowerFromPerceptualRoughness(float perceptualRoughness)
{
	float roughness = RoughnessFromPerceptualRoughness(perceptualRoughness);
	return (2.0 / max(FLT_EPSILON, roughness * roughness)) - 2.0;
}

float PerceptualRoughnessFromSpecularPower(float specPower)
{
	float roughness = sqrt(2.0 / (specPower + 2.0));
	return PerceptualRoughnessFromRoughness(roughness);
}

float BurleyToMip(float perceptualRoughness, int mips, float NdR)
{
	//float specPower = SpecularPowerFromPerceptualRoughness(perceptualRoughness);
	//specPower /= (4 * max(NdR, FLT_EPSILON));
	//float scale = PerceptualRoughnessFromSpecularPower(specPower);
	//return scale * (mips - 1 - mipOffset);
	float fScale = perceptualRoughness * (1.7 - 0.7 * perceptualRoughness);    // approximate remap from LdotR based distribution to NdotH
	return fScale * (mips - 1 - mipOffset);
}

float3 DiffuseEnergyConserve(float diffuse, float3 specular, float metalness)
{
	return diffuse * ((1 - saturate(specular)) * (1 - metalness));
}

float4 FresnelSchlick(float4 f0, float fd90, float view)
{
	return f0 + (fd90 - f0) * pow(max(1.0f - view, 0.1f), 5.0f);
}

float3 IBL(float3 n, float3 v, float3 l, float3 surfaceColor)
{
	float3 r = normalize(reflect(-v, n));
	float3 h = normalize(l + r);
	float NdV = max(dot(n, v), 0.0);
	float NdL = max(dot(n, l), 0.0);
	float NdR = max(dot(n, r), 0.0);

	float cosD = dot(l, h);

	int mipLevels, width, height;
	cubemap.GetDimensions(0, width, height, mipLevels);

	float3 diffuseImageLighting = irradianceMap.Sample(basicSampler, n).rgb;
	//float3 diffuseImageLighting = cubemap.SampleLevel(basicSampler, r, BurleyToMip(1, mipLevels, NdR)).rgb;
	float3 specularImageLighting = cubemap.SampleLevel(basicSampler, r, BurleyToMip(pow(material.roughness, 0.5), mipLevels, NdR)).rgb;

	float4 specularColor = float4(lerp(0.04f.rrr, material.albedo, material.metalness), 1.0f);

	// Disney-Frosbite
	float energyBias = lerp(0, 0.5, 1 - material.roughness);
	float energyFactor = lerp(1.0, 1.0 / 1.51, 1 - material.roughness);
	float Fd90 = energyBias + 2.0 * cosD * cosD * (1 - material.roughness);

	float4 schlickFresnel = saturate(FresnelSchlick(specularColor, Fd90, NdV));

	float3 albedo = material.albedo * surfaceColor;

	float3 result = lerp(diffuseImageLighting * albedo, specularImageLighting * albedo, schlickFresnel.xyz);
	//float3 result = specularImageLighting * material.albedo;

	return result;
}


float3 GGX(float3 n, float3 l, float3 v)
{
	float3 h = normalize(l + v);
	float NdH = saturate(dot(n, h));

	float rough2 = material.roughness * material.roughness;
	float rough4 = max(rough2 * rough2, FLT_EPSILON);

	float d = (NdH * rough4 - NdH) * NdH + 1.0f;
	float D = rough4 / (3.14159265 * (d * d));

	float3 specularColor = lerp(0.04, material.albedo, material.metalness);
	float3 reflectivity = specularColor;
	float fresnel = 1.0;

	float NdL = saturate(dot(n, l));
	float LdH = saturate(dot(l, h));
	float NdV = saturate(dot(n, v));
	float VdH = saturate(dot(v, h));

	float3 F = reflectivity + (float3(1.0, 1.0, 1.0) - reflectivity) * pow(1 - LdH, 5);

	float G = min(1, min(2 * NdH * NdV / VdH, 2 * NdH * NdL / VdH));

	return G * D * F;
}

//--------------------------------------------------------------------------------------
// Calculate amount to blend between two cascades and the band where blending will occure.
//--------------------------------------------------------------------------------------
void CalculateBlendAmountForMap(in float4 shadowMapTextureCoord,
	in out float currentPixelsBlendBandLocation,
	out float blendBetweenCascadesAmount)
{
	// Calcaulte the blend band for the map based selection.
	float2 distanceToOne = float2 (1.0f - shadowMapTextureCoord.x, 1.0f - shadowMapTextureCoord.y);
	currentPixelsBlendBandLocation = min(shadowMapTextureCoord.x, shadowMapTextureCoord.y);
	float currentPixelsBlendBandLocation2 = min(distanceToOne.x, distanceToOne.y);
	currentPixelsBlendBandLocation =
		min(currentPixelsBlendBandLocation, currentPixelsBlendBandLocation2);
	blendBetweenCascadesAmount = currentPixelsBlendBandLocation / cascadeBlendArea;
}

void ComputeCoordinatesTransform(in int cascadeIndex,
	in float4 interpolatedPosition,
	in out float4 shadowTexCoord,
	in out float4 shadowTexCoordViewSpace)
{
	// Now that we know the correct map, we can transform the world space position of the current fragment                
	shadowTexCoord = shadowTexCoordViewSpace * cascadeScale[cascadeIndex];
	shadowTexCoord += cascadeOffset[cascadeIndex];

	shadowTexCoord.x *= shadowPartitionSize;  // precomputed (float)cascadeIndex / (float)CASCADE_CNT
	shadowTexCoord.x += (shadowPartitionSize * (float)cascadeIndex);


}

//--------------------------------------------------------------------------------------
// This function calculates the screen space depth for shadow space texels
//--------------------------------------------------------------------------------------
void CalculateRightAndUpTexelDepthDeltas(in float3 shadowTexDDX,
	in float3 shadowTexDDY,
	out float upTextDepthWeight,
	out float rightTextDepthWeight
) {

	// We use the derivatives in X and Y to create a transformation matrix.  Because these derivives give us the 
	// transformation from screen space to shadow space, we need the inverse matrix to take us from shadow space 
	// to screen space.  This new matrix will allow us to map shadow map texels to screen space.  This will allow 
	// us to find the screen space depth of a corresponding depth pixel.
	// This is not a perfect solution as it assumes the underlying geometry of the scene is a plane.  A more 
	// accureate way of finding the actual depth would be to do a deferred rendering approach and actually 
	//sample the depth.

	// Using an offset, or using variance shadow maps is a better approach to reducing these artifacts in most cases.

	float2x2 matScreentoShadow = float2x2(shadowTexDDX.xy, shadowTexDDY.xy);
	float invDeterminant = 1.0f / determinant(matScreentoShadow);

	float2x2 matShadowToScreen = float2x2 (
		matScreentoShadow._22 * invDeterminant, matScreentoShadow._12 * -invDeterminant,
		matScreentoShadow._21 * -invDeterminant, matScreentoShadow._11 * invDeterminant);

	float2 rightShadowTexelLocation = float2(texelSize, 0.0f);
	float2 upShadowTexelLocation = float2(0.0f, texelSize);

	// Transform the right pixel by the shadow space to screen space matrix.
	float2 rightTexelDepthRatio = mul(rightShadowTexelLocation, matShadowToScreen);
	float2 upTexelDepthRatio = mul(upShadowTexelLocation, matShadowToScreen);

	// We can now caculate how much depth changes when you move up or right in the shadow map.
	// We use the ratio of change in x and y times the dervivite in X and Y of the screen space 
	// depth to calculate this change.
	upTextDepthWeight =
		upTexelDepthRatio.x * shadowTexDDX.z
		+ upTexelDepthRatio.y * shadowTexDDY.z;
	rightTextDepthWeight =
		rightTexelDepthRatio.x * shadowTexDDX.z
		+ rightTexelDepthRatio.y * shadowTexDDY.z;

}

//--------------------------------------------------------------------------------------
// Use PCF to sample the depth map and return a percent lit value.
//--------------------------------------------------------------------------------------
void CalculatePCFPercentLit(in float4 shadowTexCoord,
	in float rightTexelDepthDelta,
	in float upTexelDepthDelta,
	in float blurRowSize,
	in int shadowMapId,
	out float percentLit
)
{
	percentLit = 0.0f;
	// This loop could be unrolled, and texture immediate offsets could be used if the kernel size were fixed.
	// This would be performance improvment.
	for (int x = pcfBlurForLoopStart; x < pcfBlurForLoopEnd; ++x)
	{
		for (int y = pcfBlurForLoopStart; y < pcfBlurForLoopEnd; ++y)
		{
			float depthcompare = shadowTexCoord.z;
			// A very simple solution to the depth bias problems of PCF is to use an offset.
			// Unfortunately, too much offset can lead to Peter-panning (shadows near the base of object disappear )
			// Too little offset can lead to shadow acne ( objects that should not be in shadow are partially self shadowed ).
			//depthcompare -= shadowBias;

			// Add in derivative computed depth scale based on the x and y pixel.
			depthcompare += rightTexelDepthDelta * ((float)x) + upTexelDepthDelta * ((float)y);

			// Compare the transformed pixel depth to the depth read from the map.
			percentLit += shadowMap.SampleCmpLevelZero(shadowSampler,
				float3(
					shadowTexCoord.x + (((float)x) * nativeTexelSizeInX),
					shadowTexCoord.y + (((float)y) * texelSize),
					float(shadowMapId)
					),
				depthcompare);
		}
	}
	percentLit /= (float)blurRowSize;
}

float4 main(VertexToPixel input) : SV_TARGET
{
	float3 v = normalize(float4(CameraPosition, 1.0f) - input.worldPos).xyz;
	float3 n = normalize(input.normal);
	float3 t = normalize(input.tangent - dot(input.tangent, n) * n);
	//float3 b = normalize(input.bitangent - dot(input.bitangent, n) * n);
	float3 b = normalize(cross(n, t));

	float3 l;
	float4 result = float4(0, 0, 0, 0);
	float4 directLighting = float4(0, 0, 0, 0);
	float4 indirectLighting = float4(0, 0, 0, 0);

	if (material.hasNormalMap)
	{
		// Normal Mapping
		float4 normalMapping = normalTexture.Sample(basicSampler, input.uv);
		normalMapping = normalMapping * 2 - 1;
		//float4 normalMapping = float4(0.0f, 0.0f, 1.0f, 0.0f);
		float3x3 TBN = float3x3(t, b, n);

		n = mul(normalMapping.xyz, TBN);
		n = normalize(n);
	}

	float4 surfaceColor;

	if (material.hasDiffuseTexture)
	{
		surfaceColor = diffuseTexture.Sample(basicSampler, input.uv) * float4(material.albedo.xyz, 1.0);
		// Gamma correction
		//surfaceColor.xyz = pow(surfaceColor.xyz, 2.2);
	}
	else
		surfaceColor = float4(material.albedo.xyz, 1.0);

	float spotAmount = 1.0;

	// Cubemap Reflection
	float4 sampleDirection = float4(reflect(-v, n), 0.0);

	float4 diffuse = float4(0, 0, 0, 0);
	float4 specular = float4(0, 0, 0, 0);

	for (int i = 0; i < lightCount; ++i)
	{
		float intensity = lights[i].Intensity;
		[call] switch (lights[i].Type)
		{
		case 0:
			// Directional
			l = normalize(-lights[i].Direction);
			break;
		case 1:
			// Point
			{
				l = input.worldPos.xyz - lights[i].Position;
				float dist = length(l);
				float att = saturate(1.0 - dist * dist / (lights[i].Range * lights[i].Range));
				att *= att;
				intensity = att * intensity;
				l = normalize(l);
			}
			break;
		case 2:
			// Spot
			{
				l = input.worldPos.xyz - lights[i].Position;
				float dist = length(l);
				float att = saturate(1.0 - dist * dist / (lights[i].Range * lights[i].Range));
				att *= att;
				intensity = att * intensity;
				l = normalize(l);
				float angleFromCenter = max(dot(l, normalize(lights[i].Direction)), 0.0f);
				if (angleFromCenter < lights[i].SpotFalloff)
					intensity = 0.0;
				spotAmount = 1.0 - (1.0 - angleFromCenter) * 1.0 / (1.0 - lights[i].SpotFalloff);
				spotAmount = saturate(spotAmount);
				//intensity = spotAmount * intensity;
			}
			break;
		}
		float NdL = saturate(dot(n, l));
		float NdV = saturate(dot(n, v));

		float lighting = 1;
		if (lights[i].castShadow == 1 && lights[i].shadowID < 8)
		{
			//// Only cast shadow for light 0
			//float2 shadowTexCoords;
			//float4 lSpacePos = mul(input.lViewSpacePos, cascadeProjection);
			//shadowTexCoords.x = 0.5f + (lSpacePos.x / lSpacePos.w * 0.5f);
			//shadowTexCoords.y = 0.5f - (lSpacePos.y / lSpacePos.w * 0.5f);
			//float pixelDepth = lSpacePos.z / lSpacePos.w;

			//// Check if the pixel texture coordinate is in the view frustum of the 
			//// light before doing any shadow work.
			//if ((saturate(shadowTexCoords.x) == shadowTexCoords.x) &&
			//	(saturate(shadowTexCoords.y) == shadowTexCoords.y) &&
			//	(pixelDepth > 0))
			//{
			//	// Use the SampleCmpLevelZero Texture2D method (or SampleCmp) to sample from 
			//	// the shadow map.
			//	lighting = float(shadowMap.SampleCmpLevelZero(
			//		shadowSampler,
			//		shadowTexCoords,
			//		pixelDepth));
			//}

			float4 shadowMapTextureCoord = 0.0f;
			float4 shadowMapTextureCoordBlend = 0.0f;

			float percentLit = 0.0f;
			float percentLitBlend = 0.0f;


			float upTextDepthWeight = 0;
			float rightTextDepthWeight = 0;
			float upTextDepthWeightBlend = 0;
			float rightTextDepthWeightBlend = 0;

			int iBlurRowSize = pcfBlurForLoopEnd - pcfBlurForLoopStart;
			iBlurRowSize *= iBlurRowSize;
			float blurRowSize = (float)iBlurRowSize;

			int cascadeFound = 0;
			int nextCascadeIndex = 1;

			float currentPixelDepth;

			int currentCascadeIndex;

			float4 shadowMapTextureCoordViewSpace = input.lViewSpacePos;

			currentCascadeIndex = 0;

			for (int cascadeIndex = 0; cascadeIndex < cascadeLevels && cascadeFound == 0; ++cascadeIndex)
			{
				shadowMapTextureCoord = shadowMapTextureCoordViewSpace * cascadeScale[cascadeIndex];
				shadowMapTextureCoord += cascadeOffset[cascadeIndex];

				if (min(shadowMapTextureCoord.x, shadowMapTextureCoord.y) > minBorderPadding
					&& max(shadowMapTextureCoord.x, shadowMapTextureCoord.y) < maxBorderPadding)
				{
					currentCascadeIndex = cascadeIndex;
					cascadeFound = 1;
				}
			}

			float4 color = 0;

			// Repeat text coord calculations for the next cascade. 
			// The next cascade index is used for blurring between maps.
			nextCascadeIndex = min(cascadeLevels - 1, currentCascadeIndex + 1);

			float blendBetweenCascadesAmount = 1.0f;
			float currentPixelsBlendBandLocation = 1.0f;

			CalculateBlendAmountForMap(shadowMapTextureCoord,
				currentPixelsBlendBandLocation, blendBetweenCascadesAmount);

			float3 shadowMapTextureCoordDDX;
			float3 shadowMapTextureCoordDDY;
			// The derivatives are used to find the slope of the current plane.
			// The derivative calculation has to be inside of the loop in order to prevent divergent flow control artifacts.
			shadowMapTextureCoordDDX = ddx(shadowMapTextureCoordViewSpace).xyz;
			shadowMapTextureCoordDDY = ddy(shadowMapTextureCoordViewSpace).xyz;

			shadowMapTextureCoordDDX *= cascadeScale[currentCascadeIndex].xyz;
			shadowMapTextureCoordDDY *= cascadeScale[currentCascadeIndex].xyz;


			ComputeCoordinatesTransform(currentCascadeIndex,
				input.worldPos,
				shadowMapTextureCoord,
				shadowMapTextureCoordViewSpace);


			CalculateRightAndUpTexelDepthDeltas(shadowMapTextureCoordDDX, shadowMapTextureCoordDDY,
				upTextDepthWeight, rightTextDepthWeight);

			CalculatePCFPercentLit(shadowMapTextureCoord, rightTextDepthWeight,
				upTextDepthWeight, blurRowSize, lights[i].shadowID ,percentLit);

			if (currentPixelsBlendBandLocation < cascadeBlendArea)
			{  // the current pixel is within the blend band.

				// Repeat text coord calculations for the next cascade. 
				// The next cascade index is used for blurring between maps.
				shadowMapTextureCoordBlend = shadowMapTextureCoordViewSpace * cascadeScale[nextCascadeIndex];
				shadowMapTextureCoordBlend += cascadeOffset[nextCascadeIndex];

				ComputeCoordinatesTransform(nextCascadeIndex, input.worldPos,
					shadowMapTextureCoordBlend,
					shadowMapTextureCoordViewSpace);

				// We repeat the calcuation for the next cascade layer, when blending between maps.
				if (currentPixelsBlendBandLocation < cascadeBlendArea)
				{
					// the current pixel is within the blend band.
					CalculateRightAndUpTexelDepthDeltas(shadowMapTextureCoordDDX,
						shadowMapTextureCoordDDY,
						upTextDepthWeightBlend,
						rightTextDepthWeightBlend);

					CalculatePCFPercentLit(shadowMapTextureCoordBlend, rightTextDepthWeightBlend,
						upTextDepthWeightBlend, blurRowSize, lights[i].shadowID, percentLitBlend);
					percentLit = lerp(percentLitBlend, percentLit, blendBetweenCascadesAmount);
					// Blend the two calculated shadows by the blend amount.
				}
			}
			lighting = percentLit;
		}

		float4 lightColor = float4(lights[i].Color.xyz, 0.0);
		intensity = saturate(intensity * spotAmount) * lighting;
		//intensity = saturate(intensity * spotAmount);

		float diffuseFactor = NdL;
		float4 specularColor = NdL * float4(GGX(n, l, v), 0.0f) * lightColor * intensity;

		specular += specularColor;

		float3 diffuseColor = DiffuseEnergyConserve(diffuseFactor, specularColor.xyz, material.metalness);
		diffuse += float4(diffuseColor, 0.0) * lightColor * saturate(intensity * spotAmount);

	}

	result = surfaceColor * diffuse + specular + float4(IBL(n, v, l, surfaceColor.rgb), 0.0f);
	result.w = surfaceColor.w;

	return saturate(result);

	//return diffuse;
}