#include "DSFDirect3D.h"
#include "DSFLogging.h"

DSFDirect3D* FDirect3D = nullptr;

DSFDirect3D::DSFDirect3D()
{
	FDirect3D = this;

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
	SAFE_RELEASE(depthStencilState);
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

	hr = CreateDepthStencilState();
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

	hr = CreateDepthStencilState();
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

void DSFDirect3D::ClearRenderTarget(float r, float g, float b, float a)
{
	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { r, g, b, a };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}

void DSFDirect3D::Render(Camera* camera, MeshRenderer* meshRenderer)
{
	Material* material = meshRenderer->GetMaterial();
	Mesh* mesh = meshRenderer->GetMesh();

	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 itWorldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	DirectX::XMStoreFloat4x4(&worldMatrix, meshRenderer->object->transform->GetGlobalWorldMatrix());
	DirectX::XMStoreFloat4x4(&itWorldMatrix, meshRenderer->object->transform->GetGlobalInverseTransposeWorldMatrix());
	DirectX::XMStoreFloat4x4(&viewMatrix, XMMatrixInverse(nullptr, camera->transform->GetGlobalWorldMatrix()));
	DirectX::XMStoreFloat4x4(&projectionMatrix, camera->GetProjectionMatrix());


	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	material->GetVertexShaderPtr()->SetMatrix4x4("world", worldMatrix);
	material->GetVertexShaderPtr()->SetMatrix4x4("itworld", itWorldMatrix);
	material->GetVertexShaderPtr()->SetMatrix4x4("view", viewMatrix);
	material->GetVertexShaderPtr()->SetMatrix4x4("projection", projectionMatrix);

	void* materialData;
	const size_t materialSize = material->GetMaterialStruct(&materialData);
	// Material Data
	material->GetPixelShaderPtr()->SetData(
		"material",
		materialData,
		int(materialSize)
	);

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	material->GetVertexShaderPtr()->CopyAllBufferData();
	material->GetPixelShaderPtr()->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	material->GetVertexShaderPtr()->SetShader();
	material->GetPixelShaderPtr()->SetShader();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	ID3D11Buffer* vertexBuffer = mesh->GetVertexBuffer();
	ID3D11Buffer* indexBuffer = mesh->GetIndexBuffer();
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	context->DrawIndexed(
		mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
}

void DSFDirect3D::RenderSkybox(Camera* camera)
{
	// Render Skybox
	Skybox* skybox = camera->GetSkybox();

	DirectX::XMFLOAT4X4 worldMat{};
	DirectX::XMFLOAT4X4 viewMat{};
	DirectX::XMFLOAT4X4 projMat{};
	XMStoreFloat4x4(&viewMat, XMMatrixInverse(nullptr, camera->transform->GetGlobalWorldMatrix()));
	XMStoreFloat4x4(&projMat, camera->GetProjectionMatrix());
	XMStoreFloat4x4(&worldMat, DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslationFromVector(camera->transform->GetGlobalTranslation())));

	bool result;
	result = skybox->GetVertexShader()->SetMatrix4x4("world", worldMat);
	if (!result) LOG_WARNING << "Error setting parameter " << "world" << " to skybox vertex shader. Variable not found." << std::endl;

	result = skybox->GetVertexShader()->SetMatrix4x4("view", viewMat);
	if (!result) LOG_WARNING << "Error setting parameter " << "view" << " to skybox vertex shader. Variable not found." << std::endl;

	result = skybox->GetVertexShader()->SetMatrix4x4("projection", projMat);
	if (!result) LOG_WARNING << "Error setting parameter " << "projection" << " to vertex skybox shader. Variable not found." << std::endl;

	// Sampler and Texture
	result = skybox->GetPixelShader()->SetSamplerState("basicSampler", skybox->GetSamplerState());
	if (!result) LOG_WARNING << "Error setting sampler state " << "basicSampler" << " to skybox pixel shader. Variable not found." << std::endl;
	result = skybox->GetPixelShader()->SetShaderResourceView("cubemapTexture", skybox->GetCubeMapSRV());
	if (!result) LOG_WARNING << "Error setting shader resource view " << "cubemapTexture" << " to skybox pixel shader. Variable not found." << std::endl;

	skybox->GetVertexShader()->CopyAllBufferData();
	skybox->GetPixelShader()->CopyAllBufferData();

	skybox->GetVertexShader()->SetShader();
	skybox->GetPixelShader()->SetShader();

	ID3D11Buffer* vertexBuffer = skybox->GetVertexBuffer();
	ID3D11Buffer* indexBuffer = skybox->GetIndexBuffer();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(36, 0, 0);
}

void DSFDirect3D::Present()
{
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

ID3D11Device* DSFDirect3D::GetDevice() const
{
	return device;
}

ID3D11DeviceContext* DSFDirect3D::GetDeviceContext() const
{
	return context;
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

HRESULT DSFDirect3D::CreateDepthStencilState()
{
	// Depth Stencil Test
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	// Stencil test parameters
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	HRESULT hr = device->CreateDepthStencilState(&dsDesc, &depthStencilState);
	if (FAILED(hr)) return hr;
	context->OMSetDepthStencilState(depthStencilState, 0);
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
