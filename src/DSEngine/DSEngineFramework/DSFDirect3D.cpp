#include "DSFDirect3D.h"
#include "DSFLogging.h"
#include <iostream>
#include <sstream>

DSFDirect3D::DSFDirect3D()
{
	device = nullptr;
	context = nullptr;
	swapChain = nullptr;
	backBufferRTV = nullptr;
	depthStencilView = nullptr;
	dxFeatureLevel = {};
}


DSFDirect3D::~DSFDirect3D()
{
	SAFE_RELEASE(device);
	SAFE_RELEASE(context);
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(backBufferRTV);
	SAFE_RELEASE(depthStencilView);
}

HRESULT DSFDirect3D::Init(HWND hWnd, unsigned int screenWidth, unsigned int screenHeight)
{
	width = screenWidth;
	height = screenHeight;
	this->hWnd = hWnd;
	HRESULT hr = CreateDeviceAndSwapBuffer();
	if (FAILED(hr)) return hr;

	hr = CreateRenderTargetView();
	if (FAILED(hr)) return hr;

	hr = CreateDepthStencilView();
	if (FAILED(hr)) return hr;

	// Bind the views to the pipeline, so rendering properly 
	// uses their underlying textures
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);

	// Lastly, set up a viewport so we render into
	// to correct portion of the window
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = float(width);
	viewport.Height = float(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);

	LOG_TRACE << "DS Engine Framework for Direct3D Initialized!";
	// Return the "everything is ok" HRESULT value
	return S_OK;
}

HRESULT DSFDirect3D::OnResize(unsigned int screenWidth, unsigned int screenHeight)
{
	width = screenWidth;
	height = screenHeight;

	if (!device)
	{
		LOG_WARNING << "Direct3D Device & Context not initialized. Skipping resize.";
		return S_FALSE;
	}

	// Release existing DirectX views and buffers
	if (depthStencilView) { depthStencilView->Release(); }
	if (backBufferRTV) { backBufferRTV->Release(); }

	HRESULT hr = ResizeSwapBuffers();
	if (FAILED(hr)) return hr;

	hr = CreateRenderTargetView();
	if (FAILED(hr)) return hr;

	hr = CreateDepthStencilView();
	if (FAILED(hr)) return hr;

	// Bind the views to the pipeline, so rendering properly 
	// uses their underlying textures
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);

	// Lastly, set up a viewport so we render into
	// to correct portion of the window
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = float(width);
	viewport.Height = float(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);

	LOG_TRACE << "DS Engine Framework for Direct3D Resize, width = " << width << ", height = " << height;
	// Return the "everything is ok" HRESULT value
	return S_OK;
}

void DSFDirect3D::Draw(const float deltaTime, const float totalTime)
{
	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);




	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);

	// Bind the views to the pipeline
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
}

HWND DSFDirect3D::GetWindowHandle() const
{
	return hWnd;
}

unsigned int DSFDirect3D::GetWindowWidth() const
{
	return width;
}

unsigned int DSFDirect3D::GetWindowHeight() const
{
	return height;
}

D3D_FEATURE_LEVEL DSFDirect3D::GetD3DFeatureLevel() const
{
	return dxFeatureLevel;
}

HRESULT DSFDirect3D::CreateDeviceAndSwapBuffer()
{
	// This will hold options for DirectX initialization
	unsigned int deviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// If we're in debug mode in visual studio, we also
	// want to make a "Debug DirectX Device" to see some
	// errors and warnings in Visual Studio's output window
	// when things go wrong!
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Create a description of how our swap
	// chain should work
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.Flags = 0;
	swapDesc.OutputWindow = hWnd;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Windowed = true;

	// Attempt to initialize DirectX
	return D3D11CreateDeviceAndSwapChain(
		nullptr,				// Video adapter (physical GPU) to use, or null for default
		D3D_DRIVER_TYPE_HARDWARE,	// We want to use the hardware (GPU)
		nullptr,				// Used when doing software rendering
		deviceFlags,				// Any special options
		nullptr,		// Optional array of possible versions we want as fallback
		0,				// The number of fallback in the above param
		D3D11_SDK_VERSION,			// Current version of the SDK
		&swapDesc,					// Address of swap chain options
		&swapChain,					// Pointer to our Swap Chain pointer
		&device,					// Pointer to our Device pointer
		&dxFeatureLevel,			// This will hold the actual feature level the app will use
		&context);					// Pointer to our Device Context pointer
}

HRESULT DSFDirect3D::CreateRenderTargetView()
{
	// The above function created the back buffer render target
	// for us, but we need a reference to it
	ID3D11Texture2D* backBufferTexture;
	HRESULT hr = swapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBufferTexture));

	if (FAILED(hr)) return hr;
	// Now that we have the texture, create a render target view
	// for the back buffer so we can render into it.  Then release
	// our local reference to the texture, since we have the view.
	hr = device->CreateRenderTargetView(
		backBufferTexture,
		nullptr,
		&backBufferRTV);
	backBufferTexture->Release();
	return hr;
}

HRESULT DSFDirect3D::CreateDepthStencilView()
{
	// Set up the description of the texture to use for the depth buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	// Create the depth buffer and its view, then 
	// release our reference to the texture
	ID3D11Texture2D* depthBufferTexture;
	HRESULT hr = device->CreateTexture2D(&depthStencilDesc, nullptr, &depthBufferTexture);
	if (FAILED(hr)) return hr;
	hr = device->CreateDepthStencilView(depthBufferTexture, nullptr, &depthStencilView);
	depthBufferTexture->Release();
	return hr;
}

HRESULT DSFDirect3D::ResizeSwapBuffers() const
{
	// Resize the underlying swap chain buffers
	return swapChain->ResizeBuffers(
		2,
		width,
		height,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		0);
}
