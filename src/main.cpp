#include "Global.h"

#include "Utils/FunctionCalling.h"

#include "DirectXHook.h"


DWORD WINAPI Main(LPVOID lpThreadParameter){
    //LOGGING
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    auto console = spdlog::stdout_color_mt("console");
    auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("f4mp_logger", "logs/f4mp.txt");

    spdlog::set_default_logger(async_file);

    spdlog::get("console")->info("F4MP Console Loaded");


    Hooks::DirectX::Init();


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

