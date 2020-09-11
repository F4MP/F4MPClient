//
// Created by again on 8/21/2020.
//

#include "Window.h"

#include "Window.h"
#include <utility>
#include <F4MPReverse/Exceptions.h>

using namespace Exceptions::Core::Exceptions;

Window::Window(std::string windowClassName) : temp_window(nullptr), window_class_name(std::move(windowClassName))
{
    ZeroMemory(&window_class, sizeof(WNDCLASSEX));

    window_class.cbSize = sizeof(WNDCLASSEX);
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc = DefWindowProc;
    window_class.lpszClassName = window_class_name.c_str();

    window_class.hInstance = GetModuleHandle(nullptr);
    if (window_class.hInstance == nullptr)
    {
        throw GenericWinAPIException("Could not get the instance handle");
    }

    if (!RegisterClassEx(&window_class))
    {
        throw GenericWinAPIException("Could not get register the window class");
    }

    temp_window = CreateWindow(window_class.lpszClassName, "Temporary DirectX Overlay Window",
                               WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, window_class.hInstance, NULL);
    if (temp_window == nullptr)
    {
        throw GenericWinAPIException("Could not get create the temporary window");
    }
}

Window::~Window()
{
    if (temp_window)
    {
        DestroyWindow(temp_window);
        UnregisterClass(window_class.lpszClassName, window_class.hInstance);
    }
}

HWND Window::windowHandle() const
{
    return temp_window;
}