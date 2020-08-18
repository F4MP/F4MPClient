#include <windows.h>
#include <iostream>
#include <assert.h>
#include <string>
#include <steam/isteamfriends.h>


void Main(){
    AllocConsole();

    FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

    
    std::cout << SteamFriends()->GetPersonaName() << std::endl;

}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved){

    switch(ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            CloseHandle(CreateThread(nullptr ,0 ,(LPTHREAD_START_ROUTINE)Main ,hModule ,0 ,nullptr));
             MessageBox(NULL, "I got attached", "Dll says:", MB_OK);
            break;

        case DLL_PROCESS_DETACH:
            MessageBox(NULL, "I got detached", "Dll says:", MB_OK);
            break;

        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
            
            break;
    }

    return TRUE;
}