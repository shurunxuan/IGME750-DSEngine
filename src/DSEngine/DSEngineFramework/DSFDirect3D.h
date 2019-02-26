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

#include <Windows.h>
#include <d3d11.h>
#include <string>

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
	 * @brief Draw on screen
	 * 
	 * @param deltaTime The time cost by one frame
	 * @param totalTime The time since the game started
	 */
	void Draw(float deltaTime, float totalTime);

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
	 * @brief Resize the Swap Buffer object
	 *
	 * @return HRESULT S_OK if succeed, or other
	 */
	HRESULT ResizeSwapBuffers() const;
};