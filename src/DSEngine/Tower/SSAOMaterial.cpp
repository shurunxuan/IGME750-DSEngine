#include "SSAOMaterial.h"
#include <random>

SSAOMaterial::SSAOMaterial(int sourceCount, std::vector<int> sourceIndices, int targetCount,
	std::vector<int> targetIndices, ID3D11Device* d)
	: PostProcessingMaterial(sourceCount, sourceIndices, targetCount, targetIndices, d), dis(0.0f, 1.0f)
{
	BuildOffsetVectors();
}

SSAOMaterial::SSAOMaterial(int sourceCount, std::vector<int> sourceIndices, int targetCount,
	std::vector<int> targetIndices, SimpleVertexShader* vtxShader, SimplePixelShader* pxlShader, ID3D11Device* d)
	: PostProcessingMaterial(sourceCount, sourceIndices, targetCount, targetIndices, vtxShader, pxlShader, d), dis(0.0f, 1.0f)
{
	BuildOffsetVectors();
}

SSAOMaterial::~SSAOMaterial()
{
	if (randomVectorSRV != nullptr)
		randomVectorSRV->Release();
}

void SSAOMaterial::SetCamera(Camera* camera)
{
	this->camera = camera;
}

void SSAOMaterial::SetShaderData()
{

	DirectX::XMMATRIX P = XMMatrixTranspose(camera->GetProjectionMatrix());
	DirectX::XMMATRIX T(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	DirectX::XMFLOAT4X4 projMat{};
	DirectX::XMStoreFloat4x4(&projMat, XMMatrixTranspose(P));

	DirectX::XMFLOAT4X4 invProjMat{};
	DirectX::XMStoreFloat4x4(&invProjMat, XMMatrixTranspose(XMMatrixInverse(nullptr, P)));

	DirectX::XMFLOAT4X4 projTex{};
	XMStoreFloat4x4(&projTex, XMMatrixTranspose(P*T));

	DirectX::XMFLOAT4 randomNumbers{ float(dis(e) + 1.0f), float(dis(e) + 1.0f), float(dis(e) + 1.0f), float(dis(e) + 1.0f) };

	ssaoCB.OcclusionRadius = 0.5f;
	ssaoCB.OcclusionFadeStart = 0.2f;
	ssaoCB.OcclusionFadeEnd = 1.0f;
	ssaoCB.SurfaceEpsilon = 0.05f;

	vertexShader->SetMatrix4x4("gInvProj", invProjMat);
	pixelShader->SetMatrix4x4("gProj", projMat);
	pixelShader->SetMatrix4x4("gProjTex", projTex);
	pixelShader->SetData("gOffsetVectors", &*mOffsets.begin(), unsigned(sizeof(DirectX::XMFLOAT4) * mOffsets.size()));
	pixelShader->SetFloat("gOcclusionRadius", ssaoCB.OcclusionRadius);
	pixelShader->SetFloat("gOcclusionFadeStart", ssaoCB.OcclusionFadeStart);
	pixelShader->SetFloat("gOcclusionFadeEnd", ssaoCB.OcclusionFadeEnd);
	pixelShader->SetFloat("gSurfaceEpsilon", ssaoCB.SurfaceEpsilon);
	pixelShader->SetFloat4("randomNumbers", randomNumbers);

}

size_t SSAOMaterial::GetMaterialStruct(void** mtlPtr)
{
	*mtlPtr = nullptr;
	return 0;
}

void SSAOMaterial::BuildOffsetVectors()
{
	mOffsets[0] = DirectX::XMFLOAT4(+1.0f, +1.0f, +1.0f, 0.0f);
	mOffsets[1] = DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 0.0f);

	mOffsets[2] = DirectX::XMFLOAT4(-1.0f, +1.0f, +1.0f, 0.0f);
	mOffsets[3] = DirectX::XMFLOAT4(+1.0f, -1.0f, -1.0f, 0.0f);

	mOffsets[4] = DirectX::XMFLOAT4(+1.0f, +1.0f, -1.0f, 0.0f);
	mOffsets[5] = DirectX::XMFLOAT4(-1.0f, -1.0f, +1.0f, 0.0f);

	mOffsets[6] = DirectX::XMFLOAT4(-1.0f, +1.0f, -1.0f, 0.0f);
	mOffsets[7] = DirectX::XMFLOAT4(+1.0f, -1.0f, +1.0f, 0.0f);

	// 6 centers of cube faces
	mOffsets[8] = DirectX::XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
	mOffsets[9] = DirectX::XMFLOAT4(+1.0f, 0.0f, 0.0f, 0.0f);

	mOffsets[10] = DirectX::XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	mOffsets[11] = DirectX::XMFLOAT4(0.0f, +1.0f, 0.0f, 0.0f);

	mOffsets[12] = DirectX::XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
	mOffsets[13] = DirectX::XMFLOAT4(0.0f, 0.0f, +1.0f, 0.0f);

	for (auto& mOffset : mOffsets)
	{
		// Create random lengths in [0.25, 1.0].
		const float s = float(dis(e) * 0.75f + 0.25f);

		const DirectX::XMVECTOR v = DirectX::XMVectorScale(DirectX::XMVector4Normalize(XMLoadFloat4(&mOffset)), s);

		XMStoreFloat4(&mOffset, v);
	}
}

//
//// Nonnumeric values cannot be added to a cbuffer.
//Texture2D gNormalMap : register(t2);
//Texture2D gDepthMap : register(t3);
//
//SamplerState gsamPointClamp : register(s0);
//SamplerState gsamDepthMap : register(s2);
