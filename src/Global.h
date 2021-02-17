#ifndef F4MPCLIENT_GLOBAL_H
#define F4MPCLIENT_GLOBAL_H

#include <windows.h>
#include <iostream>
#include <assert.h>
#include <string>
#include <thread>
#include <mutex>

#include <d3d11.h>

//spdlog
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>


IDXGISwapChain *g_d3d11SwapChain = nullptr;
ID3D11Device *g_d3d11Device = nullptr;
ID3D11DeviceContext *g_d3d11Context = nullptr;
ID3D11RenderTargetView *g_mainRenderTargetView = nullptr;
HWND g_windowHandle = nullptr;

BOOL g_ShowMenu = false;
BOOL g_Initialized = false;


#endif //F4MPCLIENT_GLOBAL_H
