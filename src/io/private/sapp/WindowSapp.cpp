#include "io/Window.hpp"
#include "sokol_app.h"

namespace ari
{
    namespace io
    {

		//------------------------------------------------------------------------------
	    WindowHandle CreateAriWindow(Window& window, const char* _title)
	    {
			a_assert(false);
			return WindowHandle();
	    }

		//------------------------------------------------------------------------------
		GLFWwindow* GetGlfwWindow(const WindowHandle& handle)
		{
			a_assert(false);
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
			v.width = sapp_width();
			v.height = sapp_height();

			return v;
		}

		//------------------------------------------------------------------------------
		const void* AndroidGetNativeActivity()
		{
			return sapp_android_get_native_activity();
		}

    } // namespace io
    
} // namespace ari
