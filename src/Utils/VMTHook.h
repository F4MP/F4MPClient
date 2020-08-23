// Kudos to ognik

#ifndef F4MPCLIENT_VMTHOOK_H
#define F4MPCLIENT_VMTHOOK_H

#include <cstdint>
#include <cstdlib>

template <std::size_t VMTOffset>
void PatchVMTMethod(uintptr_t base, uintptr_t dst) {
    const uintptr_t method_offset = base + (VMTOffset * sizeof(uintptr_t));

    ReprotectScope<MemPerm::ReadWriteExecute> protection(method_offset, sizeof(uintptr_t));
    PatchType<uintptr_t>(method_offset, dst);
}

template <typename T, std::size_t VMTOffset>
T CreateVMTMethodDetour(uintptr_t base, uintptr_t dst) {
    const uintptr_t method_offset = base + (VMTOffset * sizeof(uintptr_t));

    ReprotectScope<MemPerm::ReadWriteExecute> protection(method_offset, sizeof(uintptr_t));

    auto original = ReadType<uintptr_t>(method_offset);
    PatchType<uintptr_t>(method_offset, dst);

    return reinterpret_cast<T>(original);
}

#endif //F4MPCLIENT_VMTHOOK_H
