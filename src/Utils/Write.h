//
// Created by again on 8/23/2020.
//

#ifndef F4MPCLIENT_WRITE_H
#define F4MPCLIENT_WRITE_H

#include <cstdint>
#include <minwindef.h>
#include <cstdlib>
#include <memoryapi.h>

namespace Memory {
    uintptr_t SlideAddress(uintptr_t base, uintptr_t offset) {
        return base + offset;
    }

    uintptr_t SlideAddress(uintptr_t base, uintptr_t old_base, uintptr_t address) {
        return base + (address - old_base);
    }

    template <typename T>
    void PatchType(uintptr_t address, T data) {
        // Allow us to read, write and execute the memory region we're modifying
        ReprotectScope<MemPerm::ReadWriteExecute> protection(address, sizeof(T));

        // Write to the region
        T* p = reinterpret_cast<T*>(address);
        *p = data;
    }

    template <typename T>
    T ReadType(uintptr_t address) {
        return *reinterpret_cast<T*>(address);
    }

    template <typename T, typename U>
    T MakeCallable(U address) {
        return reinterpret_cast<T>(address);
    }

    enum class MemPerm : DWORD {
        NoAccess = PAGE_NOACCESS,
        Read = PAGE_READONLY,
        Execute = PAGE_EXECUTE,
        ReadWrite = PAGE_READWRITE,
        ReadWriteExecute = PAGE_EXECUTE_READWRITE,
        ReadExecute = PAGE_EXECUTE_READ,
    };

    MemPerm Reprotect(uintptr_t address, std::size_t size, MemPerm new_permissions) {
        DWORD memory_protection{};
        VirtualProtect(reinterpret_cast<LPVOID>(address), size, static_cast<DWORD>(new_permissions),
                       &memory_protection);
        return static_cast<MemPerm>(memory_protection);
    }


    //ReprotectScope<MemPerm::ReadWriteExecute> some_var_name(ADDRESS, AMOUNT_OF_BYTES);
    template <MemPerm TARGET>
    class ReprotectScope {

    public:
        ReprotectScope(uintptr_t address, std::size_t size) : address(address), size(size) {
            last_protection = Reprotect(address, size, TARGET);
        }
        ~ReprotectScope() {
            Reprotect(address, size, last_protection);
        }

        ReprotectScope(ReprotectScope const&) = delete;
        ReprotectScope operator=(ReprotectScope const) = delete;
        ReprotectScope& operator=(ReprotectScope const&) = delete;
        ReprotectScope(ReprotectScope&&) = delete;

    private:
        MemPerm last_protection{};
        uintptr_t address{};
        std::size_t size{};
    };
}

#endif //F4MPCLIENT_WRITE_H
