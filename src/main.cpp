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

static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, UINT, UINT> swapChainPresent11Hook;
static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, const DXGI_MODE_DESC*> swapChainResizeTarget11Hook;
static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT> swapChainResizeBuffers11Hook;



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
                spdlog::get("indicium")->clone("d3d11")->info("++ IDXGISwapChain::Present called");



                engine->RenderPipeline.pSwapChain = pChain;

                INVOKE_INDICIUM_GAME_HOOKED(engine, IndiciumDirect3DVersion11);
            });

            INDICIUM_EVT_PRE_EXTENSION pre;
            INDICIUM_EVT_PRE_EXTENSION_INIT(&pre, engine, engine->CustomContext);
            INDICIUM_EVT_POST_EXTENSION post;
            INDICIUM_EVT_POST_EXTENSION_INIT(&post, engine, engine->CustomContext);

            INVOKE_D3D11_CALLBACK(
                    engine,
                    EvtIndiciumD3D11PrePresent,
                    chain,
                    SyncInterval,
                    Flags,
                    &pre
            );

            const auto ret = swapChainPresent11Hook.call_orig(chain, SyncInterval, Flags);

            INVOKE_D3D11_CALLBACK(
                    engine,
                    EvtIndiciumD3D11PostPresent,
                    chain,
                    SyncInterval,
                    Flags,
                    &post
            );

            return ret;
        });

        
        LOG("Hooking IDXGISwapChain::ResizeTarget");

        swapChainResizeTarget11Hook.apply(vtable[DXGIHooking::ResizeTarget], [](
                IDXGISwapChain* chain,
                const DXGI_MODE_DESC* pNewTargetParameters
        ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []()
            {
                spdlog::get("indicium")->clone("d3d11")->info("++ IDXGISwapChain::ResizeTarget called");
            });

            INDICIUM_EVT_PRE_EXTENSION pre;
            INDICIUM_EVT_PRE_EXTENSION_INIT(&pre, engine, engine->CustomContext);
            INDICIUM_EVT_POST_EXTENSION post;
            INDICIUM_EVT_POST_EXTENSION_INIT(&post, engine, engine->CustomContext);

            INVOKE_D3D11_CALLBACK(
                    engine,
                    EvtIndiciumD3D11PreResizeTarget,
                    chain,
                    pNewTargetParameters,
                    &pre
            );

            const auto ret = swapChainResizeTarget11Hook.call_orig(chain, pNewTargetParameters);

            INVOKE_D3D11_CALLBACK(
                    engine,
                    EvtIndiciumD3D11PostResizeTarget,
                    chain,
                    pNewTargetParameters,
                    &post
            );

            return ret;
        });

        logger->info("Hooking IDXGISwapChain::ResizeBuffers");

        swapChainResizeBuffers11Hook.apply(vtable[DXGIHooking::ResizeBuffers], [](
                IDXGISwapChain* chain,
                UINT            BufferCount,
                UINT            Width,
                UINT            Height,
                DXGI_FORMAT     NewFormat,
                UINT            SwapChainFlags
        ) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, []()
            {
                spdlog::get("indicium")->clone("d3d11")->info("++ IDXGISwapChain::ResizeBuffers called");
            });

            INDICIUM_EVT_PRE_EXTENSION pre;
            INDICIUM_EVT_PRE_EXTENSION_INIT(&pre, engine, engine->CustomContext);
            INDICIUM_EVT_POST_EXTENSION post;
            INDICIUM_EVT_POST_EXTENSION_INIT(&post, engine, engine->CustomContext);

            INVOKE_D3D11_CALLBACK(engine, EvtIndiciumD3D11PreResizeBuffers, chain,
                                  BufferCount, Width, Height, NewFormat, SwapChainFlags, &pre);

            const auto ret = swapChainResizeBuffers11Hook.call_orig(chain,
                                                                    BufferCount, Width, Height, NewFormat, SwapChainFlags);

            INVOKE_D3D11_CALLBACK(engine, EvtIndiciumD3D11PostResizeBuffers, chain,
                                  BufferCount, Width, Height, NewFormat, SwapChainFlags, &post);

            return ret;
        });
    }
    catch (DetourException& ex)
    {
        logger->error("Hooking D3D11 failed: {}", ex.what());
    }
    catch (ModuleNotFoundException& ex)
    {
        logger->warn("Module not found: {}", ex.what());
    }
    catch (RuntimeException& ex)
    {
        logger->error("D3D11 runtime error: {}", ex.what());
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

