#include <windows.h>
#include <iostream>
#include <assert.h>
#include <string>
#include <thread>
#include <steam/steam_api.h>

#include "Hooks/Direct3D11.h"
#include "ThirdParty/Hook.h"
#include "ThirdParty/Helpers.h"

#include <mutex>

#include <d3d11.h>

static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, UINT, UINT> swapChainPresent11Hook;
static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, const DXGI_MODE_DESC*> swapChainResizeTarget11Hook;
static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT> swapChainResizeBuffers11Hook;

IDXGISwapChain *pSwapChain;
ID3D11Device *g_device;
ID3D11DeviceContext *g_context;

using namespace F4MP::Core::Exceptions;

DWORD WINAPI Main(LPVOID lpThreadParameter){


    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    std::cout << "~~CONSOLE LOADED~~" << std::endl;

    try
    {
        const std::unique_ptr<hDirect3D11::Direct3D11> d3d11(new hDirect3D11::Direct3D11);
        auto vtable = d3d11->vtable();
        

        LOG("Hooking IDXGISwapChain::Present");

        swapChainPresent11Hook.apply(vtable[hDXGI::Present], [](
                IDXGISwapChain* chain,
                UINT SyncInterval,
                UINT Flags
        ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, [&pChain = chain]()
            {
                LOG("++ IDXGISwapChain::Present called");

                pSwapChain = pChain;

                LOG("SwapChain: 0x" + std::to_string((DWORD)pChain));

                if (SUCCEEDED(pChain->GetDevice(__uuidof(ID3D11Device), (void **)&g_device))) {
                    pChain->GetDevice(__uuidof(g_device), (void**)&g_device);

                    g_device->GetImmediateContext(&g_context);

                    LOG("Context: 0x" + std::to_string((DWORD)g_context));
                    LOG("Device: 0x" + std::to_string((DWORD)g_device));
                }

            });

           const auto ret = swapChainPresent11Hook.call_orig(chain, SyncInterval, Flags);

           return ret;
        });


       
    }
    catch (DetourException& ex)
    {
        LOGERR("Hooking D3D11 failed: " + std::string(ex.what()));
    }
    catch (ModuleNotFoundException& ex)
    {
        LOGERR("Module not found: " + std::string(ex.what()));
    }
    catch (RuntimeException& ex)
    {
        LOGERR("D3D11 runtime error: " + std::string(ex.what()));
    }


    return TRUE;
}

BOOL WINAPI Detach(){

    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved){

    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls( hModule );

        if ( auto handle = CreateThread( nullptr, NULL, Main, hModule, NULL, nullptr ) )
        {
            CloseHandle( handle );
        }
    }else if(ul_reason_for_call == DLL_PROCESS_DETACH && !lpReserved){
        return Detach();
    }

    return TRUE;
}

