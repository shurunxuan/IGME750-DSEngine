#include <iostream>
#include <boost/thread.hpp>
#include "DSEngineApp.h"
#include "DSFLogging.h"

DSEngineApp* App = nullptr;

bool simulatePhysics = false;

DSEngineApp::DSEngineApp()
{
	// Since the game should initialize its own derived
	// instance before the program entry point, this
	// is called before the DSEngine function.
	// Thus, it is safe to use App pointer inside DSEngine
	// function.
	App = this;

	InitLogger();

	device = nullptr;
	context = nullptr;

	vertexShader = nullptr;
	pbrPixelShader = nullptr;

	threadPool = nullptr;
}


DSEngineApp::~DSEngineApp()
{
	delete vertexShader;
	delete pbrPixelShader;
	delete threadPool;
	StopLogger();
}

bool DSEngineApp::Initialize(HINSTANCE hInstance, LPWSTR lpCmdLine, HWND hWnd, int screenWidth, int screenHeight)
{
	unsigned int threads = boost::thread::hardware_concurrency();
	if (threads < 4) threads = 4;
	threadPool = new DSFThreadPool(threads - 1);

	width = screenWidth;
	height = screenHeight;

	audioSystem.Init();
	renderingSystem.Init(hWnd, screenWidth, screenHeight);
	inputSystem.Init(hWnd);

	threadPool->Enqueue(
		[this]()
		{
			LOG_TRACE << "High precision joystick input polling thread working at 0x" << boost::this_thread::get_id();
			while (!threadPool->stop)
			{
				auto nextTime = boost::chrono::high_resolution_clock::now() + boost::chrono::milliseconds(4);

				inputSystem.AsyncUpdate(0.004f);

				boost::this_thread::sleep_until(nextTime);
			}

		});

	device = FDirect3D->GetDevice();
	context = FDirect3D->GetDeviceContext();

	currentScene.SetD3D11Device(device, context);

	// TODO: Could this be done by a resource manager?
	vertexShader = new SimpleVertexShader(device, context);
	pbrPixelShader = new SimplePixelShader(device, context);

	vertexShader->LoadShaderFile(L"VertexShader.cso");
	pbrPixelShader->LoadShaderFile(L"PBRPixelShader.cso");

	currentScene.SetDefaultShader(vertexShader, pbrPixelShader);


	LOG_TRACE << "DSEngineApp Init";

	Init();

	return true;
}

void DSEngineApp::Loop()
{
	// Update frame time first
	// This is managed by the rendering system
	renderingSystem.UpdateTimer();
	renderingSystem.UpdateTitleBarStats();

	const float deltaTime = renderingSystem.GetDeltaTime();
	const float totalTime = renderingSystem.GetTotalTime();

	inputSystem.SyncUpdate(deltaTime);

	// This contains the actual game logic
	currentScene.Update(deltaTime, totalTime);

	renderingSystem.Update(deltaTime, totalTime);

	audioSystem.Update();
}

Scene* DSEngineApp::CurrentActiveScene()
{
	return &currentScene;
}