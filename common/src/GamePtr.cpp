#include <include/GamePtr.h>

#pragma warning(disable: 4073)
#pragma init_seg(lib)

static Memory::GamePtr_Manager sRelocMgr;

uintptr_t Memory::GamePtr_Manager::s_baseAddress = 0;

Memory::GamePtr_Manager::GamePtr_Manager(){
    Memory::GamePtr_Manager::s_baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
}