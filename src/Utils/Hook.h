#ifndef F4MPCLIENT_HOOK_H
#define F4MPCLIENT_HOOK_H
#pragma once

#include "Windows.h"
#include <detours.h>
#include "../Exceptions.h"


enum class CallConvention
{
    stdcall_t,
    cdecl_t
};

template <CallConvention cc, typename retn, typename convention, typename ...args>
struct convention;

template <typename retn, typename ...args>
struct convention<CallConvention::stdcall_t, retn, args...>
{
    typedef retn (__stdcall *type)(args ...);
};

template <typename retn, typename ...args>
struct convention<CallConvention::cdecl_t, retn, args...>
{
    typedef retn (__cdecl *type)(args ...);
};

template <CallConvention cc, typename retn, typename ...args>
class Hook
{
    typedef typename convention<cc, retn, args...>::type type;

    size_t orig_;
    type detour_;

    bool is_applied_;
    bool has_open_transaction_;

    void transaction_begin()
    {
        const auto result = DetourTransactionBegin();

        if (result != NO_ERROR)
        {
            if (result == ERROR_INVALID_OPERATION)
            {
                throw F4MP::Core::Exceptions::DetourException(
                        "A pending transaction already exists"
                );
            }

            throw F4MP::Core::Exceptions::DetourException("Unknown error");
        }

        has_open_transaction_ = true;
    }

    void transaction_commit()
    {
        const auto result = DetourTransactionCommit();

        if (result != NO_ERROR)
        {
            switch (result)
            {
                case ERROR_INVALID_DATA:
                    throw F4MP::Core::Exceptions::DetourException(
                            "Target function was changed by third party between steps of the transaction"
                    );

                case ERROR_INVALID_OPERATION:
                    throw F4MP::Core::Exceptions::DetourException(
                            "No pending transaction exists"
                    );

                case ERROR_INVALID_BLOCK:
                    throw F4MP::Core::Exceptions::DetourException(
                            "The function referenced is too small to be detoured"
                    );

                case ERROR_INVALID_HANDLE:
                    throw F4MP::Core::Exceptions::DetourException(
                            "The ppPointer parameter is null or points to a null pointer"
                    );

                case ERROR_NOT_ENOUGH_MEMORY:
                    throw F4MP::Core::Exceptions::DetourException(
                            "Not enough memory exists to complete the operation"
                    );

                default:
                    throw F4MP::Core::Exceptions::DetourException(
                            "Unknown error"
                    );
            }
        }

        has_open_transaction_ = false;
    }

    static void update_thread(HANDLE hThread)
    {
        const auto result = DetourUpdateThread(hThread);

        if (result != NO_ERROR)
        {
            if (result == ERROR_NOT_ENOUGH_MEMORY)
            {
                throw F4MP::Core::Exceptions::DetourException(
                        "Not enough memory to record identity of thread"
                );
            }

            throw F4MP::Core::Exceptions::DetourException("Unknown error");
        }
    }

public:
    Hook() : orig_(0), detour_(0), is_applied_(false), has_open_transaction_(false)
    {
    }

    ~Hook() noexcept(false)
    {
        if (has_open_transaction_)
        {
            const auto result = DetourTransactionAbort();

            if (result != NO_ERROR)
            {
                if (result == ERROR_INVALID_OPERATION)
                {
                    throw F4MP::Core::Exceptions::DetourException(
                            "No pending transaction exists"
                    );
                }
                throw F4MP::Core::Exceptions::DetourException("Unknown error");
            }
        }

        remove();
    }

    template <typename T>
    void apply(T pFunc, type detour)
    {
        detour_ = detour;
        orig_ = static_cast<size_t>(pFunc);

        transaction_begin();
        update_thread(GetCurrentThread());
        const auto result = DetourAttach(reinterpret_cast<void **>(&orig_), reinterpret_cast<void *>(detour_));

        if (result != NO_ERROR)
        {
            switch (result)
            {
                case ERROR_INVALID_BLOCK:
                    throw F4MP::Core::Exceptions::DetourException(
                            "The function referenced is too small to be detoured"
                    );

                case ERROR_INVALID_HANDLE:
                    throw F4MP::Core::Exceptions::DetourException(
                            "The ppPointer parameter is null or points to a null pointer"
                    );

                case ERROR_INVALID_OPERATION:
                    throw F4MP::Core::Exceptions::DetourException(
                            "No pending transaction exists"
                    );

                case ERROR_NOT_ENOUGH_MEMORY:
                    throw F4MP::Core::Exceptions::DetourException(
                            "Not enough memory exists to complete the operation"
                    );

                default:
                    throw F4MP::Core::Exceptions::DetourException("Unknown error");
            }
        }

        transaction_commit();

        is_applied_ = true;
    }

    void remove()
    {
        if (!is_applied_)
            return;

        is_applied_ = false;

        transaction_begin();
        update_thread(GetCurrentThread());
        const auto result = DetourDetach(reinterpret_cast<void **>(&orig_), reinterpret_cast<void *>(detour_));

        if (result != NO_ERROR)
        {
            switch (result)
            {
                case ERROR_INVALID_BLOCK:
                    throw F4MP::Core::Exceptions::DetourException(
                            "The function to be detached was too small to be detoured"
                    );

                case ERROR_INVALID_HANDLE:
                    throw F4MP::Core::Exceptions::DetourException(
                            "The ppPointer parameter is null or points to a null pointer"
                    );

                case ERROR_INVALID_OPERATION:
                    throw F4MP::Core::Exceptions::DetourException(
                            "No pending transaction exists"
                    );

                case ERROR_NOT_ENOUGH_MEMORY:
                    throw F4MP::Core::Exceptions::DetourException(
                            "Not enough memory exists to complete the operation"
                    );

                default:
                    throw F4MP::Core::Exceptions::DetourException(
                            "Unknown error"
                    );
            }
        }

        transaction_commit();
    }

    retn call_orig(args ... p)
    {
        return type(orig_)(p...);
    }

    bool is_applied() const
    {
        return is_applied_;
    }
};

#endif //F4MPCLIENT_HOOK_H
