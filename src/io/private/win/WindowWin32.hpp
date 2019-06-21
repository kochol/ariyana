#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace ari
{
    namespace io
    {
        struct WindowWin32
        {
			bool win32_in_create_window = false;
			int Width;
			int Height;
			bool FullScreen = false;
			bool HighDpi = false;
			float DpiScale = 1.0f;
			HWND hWnd = nullptr;
			HDC hdc = nullptr;
        };
        
    } // namespace io
    
} // namespace ari
