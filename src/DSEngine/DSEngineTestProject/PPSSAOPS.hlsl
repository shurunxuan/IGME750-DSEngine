float rand(float2 p, float param) { return frac(cos(dot(p, float2(23.14069263277926, 2.665144142690225))) * param); }
float3 rand3D(float2 p, float3 random)
{
	return float3(
		rand(p, random.x * 1000000.0f),
		rand(p, random.y * 1000000.0f),
		rand(p, random.z * 1000000.0f));
}

cbuffer cbSsao : register(b0)
{
	float4x4 gProj;
	float4x4 gProjTex;
	float4   gOffsetVectors[14];

	// Coordinates given in view space.
	float    gOcclusionRadius;
	float    gOcclusionFadeStart;
	float    gOcclusionFadeEnd;
	float    gSurfaceEpsilon;

	float4   randomNumbers;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D renderTexture0    : register(t2); // Normal
Texture2D renderTexture1    : register(t3); // Depth
Texture2D renderTexture2    : register(t4); // Darkened Color
Texture2D gRandomVecMap		: register(t5);
SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1); 
SamplerState pointSamplerClamp : register(s0);
SamplerState linearSamplerClamp : register(s1);

static const int gSampleCount = 14;

static const float2 gTexCoords[6] =
{
	float2(0.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 0.0f),
	float2(0.0f, 1.0f),
	float2(1.0f, 0.0f),
	float2(1.0f, 1.0f)
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosV : POSITION;
	float2 TexC : TEXCOORD0;
};

// Determines how much the sample point q occludes the point p as a function
// of distZ.
float OcclusionFunction(float distZ)
{
	//
	// If depth(q) is "behind" depth(p), then q cannot occlude p.  Moreover, if 
	// depth(q) and depth(p) are sufficiently close, then we also assume q cannot
	// occlude p because q needs to be in front of p by Epsilon to occlude p.
	//
	// We use the following function to determine the occlusion.  
	// 
	//
	//       1.0     -------------\
	//               |           |  \
	//               |           |    \
	//               |           |      \ 
	//               |           |        \
	//               |           |          \
	//               |           |            \
	//  ------|------|-----------|-------------|---------|--> zv
	//        0     Eps          z0            z1        
	//

	float occlusion = 0.0f;
	if (distZ > gSurfaceEpsilon)
	{
		float fadeLength = gOcclusionFadeEnd - gOcclusionFadeStart;

		// Linearly decrease occlusion from 1 to 0 as distZ goes 
		// from gOcclusionFadeStart to gOcclusionFadeEnd.	
		occlusion = saturate((gOcclusionFadeEnd - distZ) / fadeLength);
	}

	return occlusion;
}

float NdcDepthToViewDepth(float z_ndc)
{
	// z_ndc = A + B/viewZ, where gProj[2,2]=A and gProj[3,2]=B.
	float viewZ = gProj[3][2] / (z_ndc - gProj[2][2]);
	return viewZ;
}

float4 main(VertexOut pin) : SV_Target0
{
	// p -- the point we are computing the ambient occlusion for.
	// n -- normal vector at p.
	// q -- a random offset from p.
	// r -- a potential occluder that might occlude p.

	// Get viewspace normal and z-coord of this pixel.  
	float3 n = normalize(renderTexture0.SampleLevel(pointSamplerClamp, pin.TexC, 0.0f).xyz);
	float pz = renderTexture1.SampleLevel(linearSamplerClamp, pin.TexC, 0.0f).r;
	pz = NdcDepthToViewDepth(pz);

	//
	// Reconstruct full view space position (x,y,z).
	// Find t such that p = t*pin.PosV.
	// p.z = t*pin.PosV.z
	// t = p.z / pin.PosV.z
	//
	float3 p = (pz / pin.PosV.z)*pin.PosV;

	// Extract random vector and map from [0,1] --> [-1, +1].
	float3 randVec = 2.0f*rand3D(pin.TexC * randomNumbers.w, randomNumbers.xyz) - 1.0f;
	//float3 randVec = 2.0f*gRandomVecMap.SampleLevel(linearSampler, pin.TexC, 0.0f).rgb - 1.0f;

	float occlusionSum = 0.0f;
	float3 lightingSum = float3(0.0f, 0.0f, 0.0f);
	// Sample neighboring points about p in the hemisphere oriented by n.
	for (int i = 0; i < gSampleCount; ++i)
	{
		// Are offset vectors are fixed and uniformly distributed (so that our offset vectors
		// do not clump in the same direction).  If we reflect them about a random vector
		// then we get a random uniform distribution of offset vectors.
		//float3 offsetVector = normalize(rand3D(pin.TexC * i)) * (rand(pin.TexC * 3, 123.0f * i) * 0.75f + 0.25f);

		//float3 offset = reflect(offsetVector, randVec);
		float3 offset = reflect(gOffsetVectors[i].xyz, randVec);

		// Flip offset vector if it is behind the plane defined by (p, n).
		float flip = sign(dot(offset, n));

		// Sample a point near p within the occlusion radius.
		float3 q = p + flip * gOcclusionRadius * offset;

		// Project q and generate projective tex-coords.  
		float4 projQ = mul(float4(q, 1.0f), gProjTex);
		projQ /= projQ.w;

		// Find the nearest depth value along the ray from the eye to q (this is not
		// the depth of q, as q is just an arbitrary point near p and might
		// occupy empty space).  To find the nearest depth we look it up in the depthmap.

		float rz = renderTexture1.SampleLevel(linearSamplerClamp, projQ.xy, 0.0f).r;
		rz = NdcDepthToViewDepth(rz);

		float3 lightColor = renderTexture2.SampleLevel(linearSamplerClamp, projQ.xy, 0.0f).rgb;

		// Reconstruct full view space position r = (rx,ry,rz).  We know r
		// lies on the ray of q, so there exists a t such that r = t*q.
		// r.z = t*q.z ==> t = r.z / q.z

		float3 r = (rz / q.z) * q;

		//
		// Test whether r occludes p.
		//   * The product dot(n, normalize(r - p)) measures how much in front
		//     of the plane(p,n) the occluder point r is.  The more in front it is, the
		//     more occlusion weight we give it.  This also prevents self shadowing where 
		//     a point r on an angled plane (p,n) could give a false occlusion since they
		//     have different depth values with respect to the eye.
		//   * The weight of the occlusion is scaled based on how far the occluder is from
		//     the point we are computing the occlusion of.  If the occluder r is far away
		//     from p, then it does not occlude it.
		// 

		float distZ = p.z - r.z;
		float dp = max(dot(n, normalize(r - p)), 0.0f);

		float occlusion = dp * OcclusionFunction(distZ);

		occlusionSum += occlusion;
		lightingSum += lightColor;
	}

	occlusionSum /= gSampleCount;
	lightingSum /= gSampleCount;
	lightingSum *= occlusionSum * 4;
	float access = 1.0f - occlusionSum;
	access = pow(access, 6.0f);
	// Sharpen the contrast of the SSAO map to make the SSAO affect more dramatic.
	return saturate(float4(access + lightingSum.r, access + lightingSum.g, access + lightingSum.b, 1.0f));
	//return saturate(access);
}
