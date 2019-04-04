/**
 * @file DSFDirect3D.h
 * @author Victor Shu
 * @brief This file declares the framework class for Direct3D.
 * @version 0.1
 * @date 2019/02/20
 *
 * @copyright Copyright (c) 2019
 *
 */

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN

#endif
#include <Windows.h>
#include <d3d11.h>

#include "MeshRenderer.hpp"
#include "Light.hpp"
#include "Camera.hpp"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) \
   if(x != NULL)        \
   {                    \
      x->Release();     \
      x = NULL;         \
   }
#endif

#ifdef DSENGINEFRAMEWORK_EXPORTS
#define DSENGINEFRAMEWORK_API __declspec(dllexport)
#else
#define DSENGINEFRAMEWORK_API __declspec(dllimport)
#endif

/**
 * @brief The Direct3D Framework of the DS Engine
 */
class DSENGINEFRAMEWORK_API DSFDirect3D
{
public:
    /**
     * @brief Construct a new DSFDirect3D object
     * 
     * Should initialize all pointers as nullptr
     */
    DSFDirect3D();
    /**
     * @brief Destroy the DSFDirect3D object
     * 
     * Should delete or release or free all pointers
     */
    ~DSFDirect3D();

    /**
     * @brief Copy constructor of DSFDirect3D is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSFDirect3D(const DSFDirect3D& v) = delete;

    /**
     * @brief Move constructor of DSFDirect3D is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSFDirect3D(DSFDirect3D&& v) = delete;

    /**
     * @brief Copy assignment operator of DSFDirect3D is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSFDirect3D& operator=(const DSFDirect3D& v) = delete;

    /**
     * @brief Move assignment operator of DSFDirect3D is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    DSFDirect3D& operator=(DSFDirect3D&& v) = delete;

    /**
     * @brief Actual initialization of Direct3D Framework
     * 
     * @param hWnd The window handle
     * @param screenWidth Screen width
     * @param screenHeight Screen height
     * @return HRESULT S_OK if initialization succeed, or other
     */
    HRESULT Init(HWND hWnd, unsigned int screenWidth, unsigned int screenHeight);

    /**
     * @brief Resize the Direct3D stuff
     *
     * @param screenWidth Screen width
     * @param screenHeight Screen height
     * @return HRESULT S_OK if initialization succeed, or other
     */
    HRESULT OnResize(unsigned int screenWidth, unsigned int screenHeight);

    /**
     * @brief Clear the render target with a color
     * 
     * @param r Red
     * @param g Green
     * @param b Blue
     * @param a Alpha
     */
    void ClearRenderTarget(float r, float g, float b, float a);

    /**
     * @brief Set the default render target which is used to render objects
     * 
     */
    void SetDefaultRenderTarget() const;

    /**
     * @brief Clear the shadow map depth stencil view and set it as the render target
     * 
     * @param light The light source of the shadow map to be rendered
     */
    void ClearAndSetShadowRenderTarget(Light* light) const;

    /**
     * @brief Pre-process
     * 
     * Render the scene into the depth buffer (shadow mapping)
     * 
     * @param light The light source of the corresponding shadow
     * @param meshRenderer The MeshRenderer to be rendered
     * @param shadowVertexShader The vertex shader that is used to render the depth buffer
     */
    void PreProcess(Light* light, MeshRenderer* meshRenderer, SimpleVertexShader* shadowVertexShader) const;

    /**
     * @brief Render a mesh renderer
     * 
     * @param camera The main camera
     * @param meshRenderer The MeshRenderer component to be rendered
     */
    void Render(Camera* camera, MeshRenderer* meshRenderer);

    /**
     * @brief Render the skybox
     * 
     * @param camera The main camera that contains the skybox info
     */
    void RenderSkybox(Camera* camera);

    /**
     * @brief Present the render target to the screen
     * 
     */
    void Present();

    /**
     * @brief Get the Window Handle
     * 
     * @return HWND Handle to window
     */
    HWND GetWindowHandle() const;
    /**
     * @brief Get the width of the window
     * 
     * @return unsigned int Width of the window
     */
    unsigned int GetWindowWidth() const;
    /**
     * @brief Get the height of the window
     * 
     * @return unsigned int Height of the window
     */
    unsigned int GetWindowHeight() const;
    /**
     * @brief Get Direct3D feature level
     * 
     * @return D3D_FEATURE_LEVEL Direct3D feature level
     */
    D3D_FEATURE_LEVEL GetD3DFeatureLevel() const;

    /**
     * @brief Get the Direct3D 11 Device
     * 
     * @return ID3D11Device* The Direct3D 11 Device
     */
    ID3D11Device* GetDevice() const;

    /**
     * @brief Get the Direct3D 11 Device Context
     * 
     * @return ID3D11DeviceContext* The Direct3D 11 Device Context
     */
    ID3D11DeviceContext* GetDeviceContext() const;

private:
    /**
     * @brief The handle to the window itself
     */
    HWND					hWnd{};

    /**
     * @brief Width of the window's client area
     */
    unsigned int			width{};
    /**
     * @brief Height of the window's client area
     */
    unsigned int			height{};

    /**
     * @brief Direct3D Feature Level
     */
    D3D_FEATURE_LEVEL		dxFeatureLevel;
    /**
     * @brief The swap chain
     */
    IDXGISwapChain*			swapChain;
    /**
     * @brief Direct3D 11 device
     */
    ID3D11Device*			device;
    /**
     * @brief Direct3D 11 device context
     */
    ID3D11DeviceContext*	context;

    /**
     * @brief The render target view of the back buffer
     */
    ID3D11RenderTargetView* backBufferRTV;
    /**
     * @brief The depth stencil view
     */
    ID3D11DepthStencilView* depthStencilView;
    /**
     * @brief The depth stencil state
     */
    ID3D11DepthStencilState* depthStencilState;

    /**
     * @brief The render state that is used for drawing normal objects
     * 
     */
    ID3D11RasterizerState* drawingRenderState;

    /**
     * @brief The render state that is used in shadow mapping pre-processing
     * 
     */
    ID3D11RasterizerState* shadowRenderState;

    /**
     * @brief The comparison sampler which is used when rendering the shadow
     * 
     */
    ID3D11SamplerState* comparisonSampler;
	/**
	 * @brief The color that is used for clearing the render target
	 */
	DirectX::XMFLOAT4 clearColor;

    /**
     * @brief Create a Device And Swap Buffer object
     * 
     * @return HRESULT S_OK if succeed, or other
     */
    HRESULT CreateDeviceAndSwapBuffer();
    /**
     * @brief Create a Render Target View object
     * 
     * @return HRESULT S_OK if succeed, or other
     */
    HRESULT CreateRenderTargetView();
    /**
     * @brief Create a Depth Stencil View object
     * 
     * @return HRESULT S_OK if succeed, or other
     */
    HRESULT CreateDepthStencilView();

    /**
     * @brief Create a Depth Stencil State object
     * 
     * @return HRESULT S_OK if succeed, or other
     */
    HRESULT CreateDepthStencilState();

    /**
     * @brief Create render states that are used in rendering shadows and objects
     * 
     * @return HRESULT S_OK if succeed, or other
     */
    HRESULT CreateShadowAndDrawingRenderState();
    /**
     * @brief Resize the Swap Buffer object
     *
     * @return HRESULT S_OK if succeed, or other
     */
    HRESULT ResizeSwapBuffers() const;

};

/**
 * @brief The pointer that points to the singleton
 * of the Direct3D Framework
 */
extern DSENGINEFRAMEWORK_API DSFDirect3D* FDirect3D;