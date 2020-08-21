//
// Created by again on 8/21/2020.
//

#ifndef F4MPCLIENT_WINDOW_H
#define F4MPCLIENT_WINDOW_H

#include <string>
#include <Windows.h>

class Window
{
    WNDCLASSEX window_class{};
    HWND temp_window;
    std::string window_class_name;
public:
    Window(std::string windowClassName);
    HWND windowHandle() const;
    ~Window();
};


#endif //F4MPCLIENT_WINDOW_H
