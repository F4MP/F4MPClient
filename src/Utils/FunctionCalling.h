// Kudos to ognik

#ifndef F4MPCLIENT_FUNCTIONCALLING_H
#define F4MPCLIENT_FUNCTIONCALLING_H

#include <cstdint>

namespace Memory {

    enum class CallConvention {
        CDecl, ClrCall, StdCall, FastCall, ThisCall, VectorCall
    };
    template<CallConvention C, typename T, typename... ARGS>
    struct BuildCallable {
        static constexpr auto value = [] {
            if constexpr (C == CallConvention::CDecl) {
                return static_cast<T(_cdecl * )(ARGS...) > (nullptr);
            } else if constexpr (C == CallConvention::ClrCall) {
#ifndef __cplusplus_cli
                static_assert(false, "Requires /clr or /ZW command line option");
#else
                return static_cast<T(__clrcall*)(ARGS...)>(nullptr);
#endif
            } else if constexpr (C == CallConvention::StdCall) {
                return static_cast<T(__stdcall *)(ARGS...)>(nullptr);
            } else if constexpr (C == CallConvention::FastCall) {
                return static_cast<T(__fastcall *)(ARGS...)>(nullptr);
            } else if constexpr (C == CallConvention::ThisCall) {
                return static_cast<T(__thiscall *)(ARGS...)>(nullptr);
            } else if constexpr (C == CallConvention::VectorCall) {
                return static_cast<T(__vectorcall * )(ARGS...) > (nullptr);
            } else {
                static_assert(false, "Invalid call convention");
            }
        }();
    };

    ///
    ///Memory::AddressCall<Memory::CallConvention::ThisCall, return_type, parameters_type>(address, parameters);
    ///
    template<CallConvention C, typename R, typename... ARGS>
    R AddressCall(uintptr_t address, ARGS... args) {
        using CALLABLE = decltype(BuildCallable<C, R, ARGS...>::value);
        if constexpr (std::is_void_v<R>) {
            reinterpret_cast<CALLABLE>(address)(args...);
        } else {
            return reinterpret_cast<CALLABLE>(address)(args...);
        }
    }

}


#endif //F4MPCLIENT_FUNCTIONCALLING_H
