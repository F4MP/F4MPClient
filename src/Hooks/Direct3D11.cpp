//
// Created by again on 8/21/2020.
//

#include "Direct3D11.h"
#include "DXGI.h"

#include <iostream>


hDirect3D11::Direct3D11::Direct3D11() :
        pd3dDevice(nullptr), pd3dDeviceContext(nullptr), pSwapChain(nullptr)
{
    temp_window = std::make_unique<Window>("TempDirect3D11OverlayWindow");

    const auto hModDXGI = GetModuleHandle("DXGI.dll");
    const auto hModD3D11 = GetModuleHandle("D3D11.dll");

    if (hModDXGI == nullptr)
    {
        throw ModuleNotFoundException("Could not get handle to DXGI.dll");
    }

    if (hModD3D11 == nullptr)
    {
        throw ModuleNotFoundException("Could not get handle to D3D11.dll");
    }

    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    {
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = temp_window->windowHandle();
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    }

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    // Note: requesting lower feature level in case of missing hardware support
    const D3D_FEATURE_LEVEL featureLevelArray[3] = {
            D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0
    };

    const auto hD3D11CreateDeviceAndSwapChain = static_cast<LPVOID>(GetProcAddress(
            hModD3D11,
            "D3D11CreateDeviceAndSwapChain"
    ));
    if (hD3D11CreateDeviceAndSwapChain == nullptr)
    {
        std::cout << "Could not get handle to D3D11CreateDeviceAndSwapChain" << hr11 << std::endl;
    }

    const auto hr11 = static_cast<HRESULT(WINAPI *)(
            IDXGIAdapter*,
            D3D_DRIVER_TYPE,
            HMODULE,
            UINT,
            const D3D_FEATURE_LEVEL*,
            UINT,
            UINT,
            const DXGI_SWAP_CHAIN_DESC*,
            IDXGISwapChain**,
            ID3D11Device**,
            D3D_FEATURE_LEVEL*,
            ID3D11DeviceContext**)>(hD3D11CreateDeviceAndSwapChain)(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            createDeviceFlags,
            featureLevelArray,
            1,
            D3D11_SDK_VERSION,
            &sd,
            &pSwapChain,
            &pd3dDevice,
            &featureLevel,
            &pd3dDeviceContext);

    if (FAILED(hr11))
    {
        std::cout << "Could not create D3D11 device" << hr11 << std::endl;
    }
}

std::vector<size_t> hDirect3D11::Direct3D11::vtable() const
{
    return std::vector<size_t>(*reinterpret_cast<size_t**>(pSwapChain),
                               *reinterpret_cast<size_t**>(pSwapChain) + DXGIHooking::DXGI::SwapChainVTableElements);
}

hDirect3D11::Direct3D11::~Direct3D11()
{
    if (pSwapChain)
        pSwapChain->Release();

    if (pd3dDevice)
        pd3dDevice->Release();

    if (pd3dDeviceContext)
        pd3dDeviceContext->Release();
}