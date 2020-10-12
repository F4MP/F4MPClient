#ifndef F4MPREVERSE_GAMEPTR_H
#define F4MPREVERSE_GAMEPTR_H

#include <Windows.h>
#include <utility>
#include <cstdint>

namespace Memory {

    class GamePtr_Manager {
    public:
        GamePtr_Manager();

        static uintptr_t	s_baseAddress;
    };

    uintptr_t translateAddr(uintptr_t offset){
        return offset + Memory::GamePtr_Manager::s_baseAddress;
    }

    template <typename T>
    class GamePtr {
    public :
        GamePtr(uintptr_t offset) : offset(offset + Memory::GamePtr_Manager::s_baseAddress){}

        T * GetPtr() const {
            return reinterpret_cast <T *>(offset);
        }

        T * operator->() const
        {
            return GetPtr();
        }

        operator T *() const
        {
            return GetPtr();
        }

        bool operator!() const {
            return !GetPtr();
        }

        const T * GetConstPtr() const {
            return reinterpret_cast <T *>(offset);
        }

        uintptr_t GetUIntPtr() const
        {
            return offset;
        }
    private:
        uintptr_t  offset;

        GamePtr();
        GamePtr(GamePtr & rhs);
        GamePtr & operator=(GamePtr & rhs);
    };

    template <typename T>
    class GameAddr{
    public:
        GameAddr(uintptr_t offset) : offset(reinterpret_cast<ConversionType *>(offset + Memory::GamePtr_Manager::s_baseAddress)){}

        operator T() {
            return reinterpret_cast<T>(offset);
        }

        uintptr_t GetUIntPtr(){
            return reinterpret_cast<uintptr_t >(offset);
        }
    private:
        struct ConversionType {};

        ConversionType * offset;

        GameAddr();
        GameAddr(GameAddr & rhs);
        GameAddr & operator=(GameAddr & rhs);
    };

    template <typename T, uintptr_t Addr>
    class GameFunc{
    public:

        template <class ...Args>
        auto operator()(Args ... args){
            T * f = (T*)translateAddr(Addr);
            return f(std::forward<Args>(args)...);
        }
    };
}

#endif
