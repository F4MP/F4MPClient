#include "Global.h"
#include "DirectXHook.h"

#include <common/include/GamePtr.h>
#include <common/include/Utilities.h>
#include <common/include/Types.h>
#include <common/include/Hook.h>

#include "Game.h"
#include "Logger.h"

DWORD WINAPI Main(LPVOID lpThreadParameter){


	LOGGER_INIT();

	LOGGER_INFO("ss");

    Hooks::DirectX::Init();
	spdlog::get("console")->warn();


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

