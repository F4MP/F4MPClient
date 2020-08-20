//
// Created by again on 8/21/2020.
//

#ifndef F4MPCLIENT_VMTHOOK_H
#define F4MPCLIENT_VMTHOOK_H

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include "shlwapi.h"
#include <iostream>

LPVOID HookMethod(_In_ LPVOID lpVirtualTable, _In_ PVOID pHookMethod,
                  _In_opt_ uintptr_t dwOffset){
    uintptr_t dwVTable	= *((uintptr_t*)lpVirtualTable);
    uintptr_t dwEntry	= dwVTable + dwOffset;
    uintptr_t dwOrig	= *((uintptr_t*)dwEntry);

    DWORD dwOldProtection;
    ::VirtualProtect((LPVOID)dwEntry, sizeof(dwEntry),
                     PAGE_EXECUTE_READWRITE, &dwOldProtection);

    *((uintptr_t*)dwEntry) = (uintptr_t)pHookMethod;

    ::VirtualProtect((LPVOID)dwEntry, sizeof(dwEntry),
                     dwOldProtection, &dwOldProtection);

    return (LPVOID) dwOrig;
}


#endif //F4MPCLIENT_VMTHOOK_H
