#pragma once
#include <Windows.h>

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
			HWND hWnd = nullptr;
        };
        
    } // namespace io
    
} // namespace ari
