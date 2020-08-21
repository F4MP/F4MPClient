//
// Created by again on 8/21/2020.
//

#ifndef F4MPCLIENT_HELPERS_H
#define F4MPCLIENT_HELPERS_H

#include <iostream>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void _Error(std::string msg){

    SetConsoleTextAttribute(hConsole, 04);
    std::cout << msg << std::endl;
    SetConsoleTextAttribute(hConsole, 15);
}

#define LOG(msg) std::cout << msg  << std::endl;
#define ERROR(msg) _Error(msg);

#define INVOKE_D3D11_CALLBACK(_engine_, _callback_, ...)     \
                             (_engine_->EventsD3D11._callback_ ? \
                             _engine_->EventsD3D11._callback_(##__VA_ARGS__) : \
                             (void)0)

#endif //F4MPCLIENT_HELPERS_H
