#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include "io/Window.hpp"

namespace ari
{
    namespace io
    {
        struct WindowWin32 : public Window
        {
			bool win32_in_create_window = false;
			HWND hWnd = nullptr;
			HDC hdc = nullptr;
        };
        
    } // namespace io
    
} // namespace ari
