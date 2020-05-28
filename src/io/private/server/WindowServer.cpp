#include "io/Window.hpp"
#include "sokol_app.h"

namespace ari
{
    namespace io
    {

		//------------------------------------------------------------------------------
	    WindowHandle CreateAriWindow(Window& window, const char* _title)
	    {
			return WindowHandle();
	    }

		//------------------------------------------------------------------------------
		GLFWwindow* GetGlfwWindow(const WindowHandle& handle)
		{
			return nullptr;
		}

		//------------------------------------------------------------------------------
		bool Run()
		{
			return true;
		}

		//------------------------------------------------------------------------------
		core::RectI GetWindowSize(const WindowHandle& handle)
		{
			core::RectI v;
			v.width = 0;
			v.height = 0;

			return v;
		}

		//------------------------------------------------------------------------------
		const void* AndroidGetNativeActivity()
		{
			return nullptr;
		}

    } // namespace io
    
} // namespace ari
