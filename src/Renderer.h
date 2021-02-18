#ifndef F4MP_CLIENT_RENDERER_H
#define F4MP_CLIENT_RENDERER_H

#include <d3d11.h>

/*
 *
 * ID3D11DeviceContext *g_d3d11Context = nullptr;
ID3D11RenderTargetView *g_mainRenderTargetView = nullptr;
 */
class Renderer {
	ID3D11Device* d3d11Device = nullptr;
	IDXGISwapChain* d3d11SwapChain = nullptr;
	ID3D11DeviceContext* d3d11Context = nullptr;
	ID3D11RenderTargetView* mainRenderTargetView = nullptr;

	bool isInitialized = false;
public:
	Renderer();

	bool GetIsInitialized();
	ID3D11Device* GetD3D11Device();
	IDXGISwapChain* GetD3D11SwapChain();
	ID3D11DeviceContext* GetD3D11DeviceContext();
	ID3D11RenderTargetView* GetMainRenderTargetView();

protected:
	// TODO: Add info about the Render method
	virtual void Render();
};


#endif
