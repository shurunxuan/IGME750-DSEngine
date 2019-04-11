#include "DSSRendering.h"
#include "DSFLogging.h"
#include "DSEngineApp.h"
#include <iostream>

DSSRendering* SRendering = nullptr;

DSSRendering::DSSRendering()
{
	SRendering = this;

	// Query performance counter for accurate timing information
	__int64 perfFreq;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&perfFreq));
	perfCounterSeconds = 1.0 / double(perfFreq);

	shadowVertexShader = nullptr;
	postProcessingVS = nullptr;
	postProcessingCopyPS = nullptr;
	copyToScreenMaterial = nullptr;
}


DSSRendering::~DSSRendering()
{
	delete shadowVertexShader;
	delete postProcessingVS;
	delete postProcessingCopyPS;
	delete copyToScreenMaterial;
}

HRESULT DSSRendering::Init(HWND hWnd, unsigned int screenWidth, unsigned int screenHeight)
{
	HRESULT hr = direct3D.Init(hWnd, screenWidth, screenHeight);
	LOG_TRACE << "DS Engine Rendering System Initialized!";

	// Grab the start time now that
	// the game loop is going to run
	__int64 now;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&now));
	startTime = now;
	currentTime = now;
	previousTime = now;

	shadowVertexShader = new SimpleVertexShader(direct3D.GetDevice(), direct3D.GetDeviceContext());
	shadowVertexShader->LoadShaderFile(L"ShadowVS.cso");
	postProcessingVS = new SimpleVertexShader(direct3D.GetDevice(), direct3D.GetDeviceContext());
	postProcessingVS->LoadShaderFile(L"PostProcessingVS.cso");
	postProcessingCopyPS = new SimplePixelShader(direct3D.GetDevice(), direct3D.GetDeviceContext());
	postProcessingCopyPS->LoadShaderFile(L"PPCopyPS.cso");
	copyToScreenMaterial = new PostProcessingMaterial(1, { 0 }, 1, { -1 }, postProcessingVS, postProcessingCopyPS, direct3D.GetDevice());
	return hr;
}

HRESULT DSSRendering::OnResize(unsigned int screenWidth, unsigned int screenHeight)
{
	HRESULT hr = direct3D.OnResize(screenWidth, screenHeight);
	App->CurrentActiveScene()->mainCamera->UpdateProjectionMatrix(float(screenWidth), float(screenHeight), 3.1415926f / 3.0f);
	return hr;
}

void DSSRendering::Update(const float deltaTime, const float totalTime)
{
	// #66CCFF
	direct3D.ClearRenderTarget(0.4f, 0.8f, 1.0f, 1.0f);
	// Restore the blend state
	direct3D.GetDeviceContext()->OMSetBlendState(nullptr, nullptr, 0xFFFFFF);

	std::list<MeshRenderer*> meshRenderersInScene;
	std::list<MeshRenderer*> transparentMeshRenderers;
	// Get All MeshRenderers
	for (Object* object : App->CurrentActiveScene()->allObjects)
	{
		meshRenderersInScene.merge(object->GetComponents<MeshRenderer>());
	}

	// Preprocessing
	for (Light* light : App->CurrentActiveScene()->lights)
	{
		direct3D.ClearAndSetShadowRenderTarget(light);
		for (MeshRenderer* meshRenderer : meshRenderersInScene)
		{
			// TODO: if (cull(light, meshRenderer->mesh))
			direct3D.PreProcess(light, meshRenderer, shadowVertexShader);
		}
	}

	// Render the scene
	Camera* camera = App->CurrentActiveScene()->mainCamera;


	for (MeshRenderer* meshRenderer : meshRenderersInScene)
	{
		// TODO: if (cull(camera, meshRenderer->mesh)) 
		if (meshRenderer->GetMaterial()->transparent)
		{
			transparentMeshRenderers.push_back(meshRenderer);
			continue;
		}
		direct3D.Render(camera, meshRenderer);
	}


	// Render the skybox
	if (camera->GetSkybox() != nullptr)
		direct3D.RenderSkybox(camera);

	// Sort the list first
	transparentMeshRenderers.sort(
		[camera](MeshRenderer * a, MeshRenderer * b)
		{
			DirectX::XMVECTOR aBoxCenter = DirectX::XMLoadFloat3(&(a->GetMesh()->aabb.Center));
			DirectX::XMVECTOR bBoxCenter = DirectX::XMLoadFloat3(&(b->GetMesh()->aabb.Center));

			const DirectX::XMMATRIX wvA = DirectX::XMMatrixMultiply(XMMatrixTranspose(a->object->transform->GetGlobalWorldMatrix()), DirectX::XMMatrixTranspose(camera->GetViewMatrix()));
			const DirectX::XMMATRIX wvB = DirectX::XMMatrixMultiply(XMMatrixTranspose(b->object->transform->GetGlobalWorldMatrix()), DirectX::XMMatrixTranspose(camera->GetViewMatrix()));

			aBoxCenter = DirectX::XMVector3TransformCoord(aBoxCenter, wvA);
			bBoxCenter = DirectX::XMVector3TransformCoord(bBoxCenter, wvB);

			DirectX::XMFLOAT3 ac{};
			DirectX::XMFLOAT3 bc{};

			DirectX::XMStoreFloat3(&ac, aBoxCenter);
			DirectX::XMStoreFloat3(&bc, bBoxCenter);

			return ac.z > bc.z;
		}
	);

	for (MeshRenderer* meshRenderer : transparentMeshRenderers)
	{
		// Set blend state
		direct3D.GetDeviceContext()->OMSetBlendState(meshRenderer->GetMaterial()->blendState, nullptr, 0xFFFFFF);
		// Render
		direct3D.Render(camera, meshRenderer);
	}

	// Post Processing
	for (PostProcessingMaterial* material : postProcessingMaterials)
	{
		direct3D.PostProcessing(material);
	}
	direct3D.PostProcessing(copyToScreenMaterial);

	direct3D.Present();
}

void DSSRendering::RegisterPostProcessing(PostProcessingMaterial* postProcessingMaterial)
{
	postProcessingMaterials.push_back(postProcessingMaterial);
}

SimpleVertexShader* DSSRendering::GetDefaultPostProcessingVertexShader() const
{
	return postProcessingVS;
}


void DSSRendering::UpdateTimer()
{
	// Grab the current time
	__int64 now;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&now));
	currentTime = now;

	// Calculate delta time and clamp to zero
	//  - Could go negative if CPU goes into power save mode 
	//    or the process itself gets moved to another core
	deltaTime = std::max(float((currentTime - previousTime) * perfCounterSeconds), 0.0f);

	// Calculate the total time from start to now
	totalTime = float((currentTime - startTime) * perfCounterSeconds);

	// Save current time for next frame
	previousTime = currentTime;
}

void DSSRendering::UpdateTitleBarStats()
{
	fpsFrameCount++;

	// Only calc FPS and update title bar once per second
	const float timeDiff = totalTime - fpsTimeElapsed;
	if (timeDiff < 1.0f)
		return;

	// How long did each frame take?  (Approx)
	const float mspf = 1000.0f / float(fpsFrameCount);

	// Quick and dirty title bar text (mostly for debugging)

	std::basic_ostringstream<TCHAR> output;
	output.precision(6);
	output << "DSEngine App" <<
		"    Width: " << direct3D.GetWindowWidth() <<
		"    Height: " << direct3D.GetWindowHeight() <<
		"    FPS: " << fpsFrameCount <<
		"    Frame Time: " << mspf << "ms";

	// Append the version of DirectX the app is using
	switch (direct3D.GetD3DFeatureLevel())
	{
	case D3D_FEATURE_LEVEL_11_1: output << "    DX 11.1"; break;
	case D3D_FEATURE_LEVEL_11_0: output << "    DX 11.0"; break;
	case D3D_FEATURE_LEVEL_10_1: output << "    DX 10.1"; break;
	case D3D_FEATURE_LEVEL_10_0: output << "    DX 10.0"; break;
	case D3D_FEATURE_LEVEL_9_3:  output << "    DX 9.3";  break;
	case D3D_FEATURE_LEVEL_9_2:  output << "    DX 9.2";  break;
	case D3D_FEATURE_LEVEL_9_1:  output << "    DX 9.1";  break;
	default:                     output << "    DX ???";  break;
	}

	// Actually update the title bar and reset fps data
	SetWindowText(direct3D.GetWindowHandle(), output.str().c_str());
	fpsFrameCount = 0;
	fpsTimeElapsed += 1.0f;
}

float DSSRendering::GetDeltaTime() const
{
	return deltaTime;
}

float DSSRendering::GetTotalTime() const
{
	return totalTime;
}
