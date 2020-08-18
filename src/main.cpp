#include <windows.h>
#include <iostream>

void Main(void){
    
    AllocConsole();

    freopen("CONOUT$","w",stdout);

    
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL,  DWORD fdwReason, LPVOID lpReserved ){

    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)Main,NULL,NULL,NULL);
            break;

        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
            
            break;
    }

    return TRUE;
}