//
// Created by again on 8/21/2020.
//

#ifndef F4MPCLIENT_DIRECT3DBASE_H
#define F4MPCLIENT_DIRECT3DBASE_H

#include "Window.h"
#include <vector>
#include <memory>


namespace hDirect3D11
{
    class Direct3DBase
    {
    protected:
        std::unique_ptr<Window> temp_window;
        virtual ~Direct3DBase() {}
    public:
        Direct3DBase() {}

        virtual std::vector<size_t> vtable() const = 0;
    };
}

#endif //F4MPCLIENT_DIRECT3DBASE_H
