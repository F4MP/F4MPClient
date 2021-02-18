#ifndef F4MPCLIENT_DIRECTXHOOK_H
#define F4MPCLIENT_DIRECTXHOOK_H

#include "Global.h"
#include <common/include/Hook.h>
//hooks
#include "Hooks/Direct3D11.h"
#include "Hooks/DXGI.h"

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

using namespace Exceptions::Core::Exceptions;
using namespace Hooks;

static Hook<CallConvention::stdcall_t, HRESULT, IDXGISwapChain*, UINT, UINT> swapChainPresent11Hook;
static WNDPROC OriginalWndProcHandler = nullptr;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


namespace Hooks {
    namespace DirectX {
        //DEF
        void Init();
        void Render();
        void Pre_Render(IDXGISwapChain* swapChain);
        void Imgui_Render_Impl();

        //IMPL
        extern LRESULT CALLBACK hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            ImGuiIO& io = ImGui::GetIO();
            POINT mPos;
            GetCursorPos(&mPos);
            ScreenToClient(g_windowHandle, &mPos);
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


            if (g_ShowMenu)
            {
                ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
                return true;
            }

            return CallWindowProc(OriginalWndProcHandler, hWnd, uMsg, wParam, lParam);
        }

        void Init(){
         /*   try
            {
                const std::unique_ptr<hDirect3D11::Direct3D11> d3d11(new hDirect3D11::Direct3D11);
                auto vtable = d3d11->vtable();


                spdlog::info("[H] IDXGISwapChain::Present");

                swapChainPresent11Hook.apply(vtable[hDXGI::Present], [](IDXGISwapChain* chain,UINT SyncInterval,UINT Flags) -> HRESULT
                {
                    static std::once_flag flag;
                    std::call_once(flag, [&pChain = chain]()
                    {
                        spdlog::info("[+H] IDXGISwapChain::Present called");
                        g_d3d11SwapChain = pChain;

                        if (!g_Initialized && SUCCEEDED(pChain->GetDevice(__uuidof(ID3D11Device), (void **)&g_d3d11Device))) {
                            Pre_Render(pChain);

                            g_Initialized = true;

                        }

                    });

                    Render();

                    const auto ret = swapChainPresent11Hook.call_orig(chain, SyncInterval, Flags);

                    return ret;
                });
            }
            catch (DetourException& ex)
            {
                spdlog::error("Hooking D3D11 failed: {}",ex.what());
            }
            catch (ModuleNotFoundException& ex)
            {
                spdlog::error("Module not found: {}",ex.what());
            }
            catch (RuntimeException& ex)
            {
                spdlog::error("D3D11 runtime error: {}",ex.what());
            }*/
        }

        void Pre_Render(IDXGISwapChain* swapChain){
            swapChain->GetDevice(__uuidof(g_d3d11Device), (void**)&g_d3d11Device);

            g_d3d11Device->GetImmediateContext(&g_d3d11Context);

            DXGI_SWAP_CHAIN_DESC sd;
            swapChain->GetDesc(&sd);
            g_windowHandle = sd.OutputWindow;
            OriginalWndProcHandler = (WNDPROC)SetWindowLongPtr(g_windowHandle, GWLP_WNDPROC, (LONG_PTR)hWndProc);

            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

            ImGui_ImplWin32_Init(g_windowHandle);
            ImGui_ImplDX11_Init(g_d3d11Device, g_d3d11Context);
            ImGui::GetIO().ImeWindowHandle = g_windowHandle;


            ID3D11Texture2D* pBackBuffer;

            swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            g_d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
            pBackBuffer->Release();

        }

        void Render(){
           Imgui_Render_Impl();


        }

        void Imgui_Render_Impl(){
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

            ImGui::Render();

            g_d3d11Context->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }
    }
}

#endif //F4MPCLIENT_DIRECTXHOOK_H
