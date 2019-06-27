#include "io/Window.hpp"
#include "sokol_app.h"

namespace ari
{
    namespace io
    {

	    WindowHandle CreateAriWindow(Window& window, const char* _title)
	    {
			a_assert(false);
			return WindowHandle();
	    }

		GLFWwindow* GetGlfwWindow(const WindowHandle& handle)
		{
			a_assert(false);
			return nullptr;
		}

		bool Run()
		{
			return true;
		}

    } // namespace io
    
} // namespace ari
