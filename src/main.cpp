#include "Global.h"


#include "DirectXHook.h"

#include <common/include/GamePtr.h>
#include <common/include/Utilities.h>
#include <common/include/Types.h>
#include <common/include/Hook.h>


static Hooks::Hook<Hooks::CallConvention::cdecl_t, void,const char *, va_list> printHook;
static Memory::GameAddr <void> printAddr(0x01262EC0);

class ConsoleManager
{
public:
    MEMBER_FN_PREFIX(ConsoleManager);
    DEFINE_MEMBER_FN(VPrint, void, 0x01262EC0, const char * fmt, va_list args);
    DEFINE_MEMBER_FN(Print, void, 0x01262F50, const char * str);
};
//00007FF6D7E30AE0
static Memory::GamePtr<ConsoleManager *>g_console(0x058E0AE0);
static Memory::GameAddr<Types::UInt32*> g_consoleHandle(0x05ADB4A8);

void Console_Print(const char * fmt, ...)
{
    ConsoleManager * mgr = *g_console;
    if(mgr)
    {
        va_list args;
                va_start(args, fmt);

        CALL_MEMBER_FN(mgr, VPrint)(fmt, args);

                va_end(args);
    }
}


void testPrint(const char * fmt, ...){
        va_list args;

        va_start(args,fmt);


        va_end(args);
}

DWORD WINAPI Main(LPVOID lpThreadParameter){
    //LOGGING
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    auto console = spdlog::stdout_color_mt("console");
    auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("f4mp_logger", "logs/f4mp.txt");

    spdlog::set_default_logger(async_file);

    spdlog::get("console")->info("F4MP Console Loaded");


    Hooks::DirectX::Init();

    printHook.apply(printAddr.GetUIntPtr(), [](const char * fmt, va_list args) -> void {
            std::cout << fmt << args << std::endl;
            return printHook.call_orig(fmt, args);
    });

    testPrint("Calling print with my own class");

    Console_Print("TEST PRINT 1");

    Console_Print("TEST PRINT 2");

    testPrint("CALLED AGAIN");

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

