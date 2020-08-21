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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include <d3d11.h>

#include <sciter/sciter-x.h>

static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, UINT, UINT> swapChainPresent11Hook;

IDXGISwapChain *pSwapChain;
ID3D11Device *g_device;
ID3D11DeviceContext *g_context;
ID3D11RenderTargetView *mainRenderTargetView;

BOOL g_Initialized = false;
BOOL g_ShowMenu = false;
HWND window = nullptr;

static WNDPROC OriginalWndProcHandler = nullptr;

BOOL InitSciterEngineInstance(HWND hwnd, IDXGISwapChain* pSwapChain);


using namespace F4MP::Core::Exceptions;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ImGuiIO& io = ImGui::GetIO();
    POINT mPos;
    GetCursorPos(&mPos);
    ScreenToClient(window, &mPos);
    ImGui::GetIO().MousePos.x = mPos.x;
    ImGui::GetIO().MousePos.y = mPos.y;
    ImGui::GetIO().MouseDrawCursor = g_ShowMenu;

    if (uMsg == WM_KEYUP)
    {
        if (wParam == VK_DELETE)
        {
            g_ShowMenu = !g_ShowMenu;
        }

    }

    LOG("Initializing SciterProcND");
    Sleep(5000);

    if (uMsg != WM_CREATE && uMsg != WM_PAINT)
    {
        BOOL handled = FALSE;
        LRESULT lr = SciterProcND(hWnd, uMsg, wParam, lParam, &handled);
    }

    if (g_ShowMenu)
    {
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
        return true;
    }

    return CallWindowProc(OriginalWndProcHandler, hWnd, uMsg, wParam, lParam);
}

DWORD WINAPI Main(LPVOID lpThreadParameter){


    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    LOG("Console Loaded");

    OleInitialize(nullptr);

    // enable "unsafe" functions to be accessible from script
    SciterSetOption(nullptr, SCITER_SET_SCRIPT_RUNTIME_FEATURES,
                    ALLOW_FILE_IO |
                    ALLOW_SOCKET_IO |
                    ALLOW_EVAL |
                    ALLOW_SYSINFO);

    try
    {
        const std::unique_ptr<hDirect3D11::Direct3D11> d3d11(new hDirect3D11::Direct3D11);
        auto vtable = d3d11->vtable();
        

        LOG("Hooking IDXGISwapChain::Present");

        swapChainPresent11Hook.apply(vtable[hDXGI::Present], [](IDXGISwapChain* chain,UINT SyncInterval,UINT Flags) -> HRESULT
        {
            static std::once_flag flag;
            std::call_once(flag, [&pChain = chain]()
            {
                LOG("++ IDXGISwapChain::Present called");

                pSwapChain = pChain;

                LOG("SwapChain: 0x" + std::to_string((DWORD)pChain));

                if (!g_Initialized && SUCCEEDED(pChain->GetDevice(__uuidof(ID3D11Device), (void **)&g_device))) {
                    pChain->GetDevice(__uuidof(g_device), (void**)&g_device);

                    g_device->GetImmediateContext(&g_context);

                    DXGI_SWAP_CHAIN_DESC sd;
                    pChain->GetDesc(&sd);

                    ImGui::CreateContext();
                    ImGuiIO& io = ImGui::GetIO(); (void)io;
                    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
                    window = sd.OutputWindow;

                    OriginalWndProcHandler = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)hWndProc);

                    ImGui_ImplWin32_Init(window);
                    ImGui_ImplDX11_Init(g_device, g_context);
                    ImGui::GetIO().ImeWindowHandle = window;

                    LOG("Initializing Sciter Engine Instance");
                    Sleep(5000);

                    if(InitSciterEngineInstance(window, pSwapChain)){
                        LOG("Sciter Engine initialized");
                    }

                    ID3D11Texture2D* pBackBuffer;

                    pChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
                    g_device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
                    pBackBuffer->Release();

                    g_Initialized = true;

                }

            });

            ImGui_ImplWin32_NewFrame();
            ImGui_ImplDX11_NewFrame();

            ImGui::NewFrame();
            //Menu is displayed when g_ShowMenu is TRUE
            if (g_ShowMenu)
            {
                bool bShow = true;
                ImGui::ShowDemoWindow(&bShow);
            }
            ImGui::EndFrame();

            SciterRenderOnDirectXWindow(window, NULL, FALSE);


            ImGui::Render();

            g_context->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

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

int gFPS = 60;


struct view_dom_event_handler : public sciter::event_handler
{
BEGIN_FUNCTION_MAP
    FUNCTION_0("FPS", FPS)
END_FUNCTION_MAP

sciter::value FPS()
{
    return sciter::value(gFPS);
}
};

view_dom_event_handler g_ViewDomEventHandler;


BOOL InitSciterEngineInstance(HWND hWnd, IDXGISwapChain *pSwapChain)
{
    // 1. create engine instance on the window with the swap chain:
    BOOL r = SciterCreateOnDirectXWindow(hWnd, pSwapChain);
    if (!r) return FALSE;

    SciterSetOption(hWnd, SCITER_SET_DEBUG_MODE, TRUE);

    // 2. setup callback (resource loading, etc):
    //SciterSetCallback(hWnd, SciterCallback, NULL);
    SciterSetCallback(hWnd,NULL,NULL);
    // 2b. setup DOM event handler:
    sciter::attach_dom_event_handler(hWnd, &g_ViewDomEventHandler);

    // 3. load HTML content in it:
    r = SciterLoadFile(hWnd, L"https://www.google.com/");

    assert(r);

    // done
    return true;
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

