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
}


DSSRendering::~DSSRendering()
{
	delete shadowVertexShader;
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

	return hr;
}

HRESULT DSSRendering::OnResize(unsigned int screenWidth, unsigned int screenHeight)
{
	HRESULT hr = direct3D.OnResize(screenWidth, screenHeight);
	App->CurrentActiveScene()->mainCamera->UpdateProjectionMatrix(float(screenWidth), float(screenHeight), 3.1415926f / 4.0f);
	return hr;
}

void DSSRendering::Update(const float deltaTime, const float totalTime)
{
	// #66CCFF
	direct3D.ClearRenderTarget(0.4f, 0.8f, 1.0f, 1.0f);

	std::list<MeshRenderer*> meshRenderersInScene;

	// Get All MeshRenderers
	for (Object* object : App->CurrentActiveScene()->allObjects)
	{
		meshRenderersInScene.merge(object->GetComponents<MeshRenderer>());
	}

	// Preprocessing
	int lightCount = 0;
	for (Light* light : App->CurrentActiveScene()->lights)
	{
		if (!light->GetData()->CastShadow) break;
		direct3D.ClearAndSetShadowRenderTarget(light, lightCount);
		for (MeshRenderer* meshRenderer : meshRenderersInScene)
		{
			// TODO: if (cull(light, meshRenderer->mesh))
			direct3D.PreProcess(light, meshRenderer, shadowVertexShader, lightCount);
		}
		++lightCount;
		if (lightCount == MAX_CAST_SHADOW_COUNT) break;
	}

	// Render the scene
	Camera* camera = App->CurrentActiveScene()->mainCamera;


	for (MeshRenderer* meshRenderer : meshRenderersInScene)
	{
		// TODO: if (cull(camera, meshRenderer->mesh)) 
		direct3D.Render(camera, meshRenderer);
	}


	// Render the skybox
	direct3D.RenderSkybox(camera);

	direct3D.Present();
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
