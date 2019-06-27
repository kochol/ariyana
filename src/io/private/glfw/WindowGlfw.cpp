#include "io/Window.hpp"
#include "io/private/flextgl/flextGL.h"
#include "GLFW/glfw3.h"
#include "core/containers/StaticArray.hpp"

namespace ari
{
    namespace io
    {
		static bool g_GlfwInited = false;
		GLFWwindow* g_FirstWindow = nullptr;
		core::StaticArray<GLFWwindow*, MaxWindow>	g_Windows;

		WindowHandle CreateAriWindow(Window& window, const char* _title)
		{
			// Init glfw
			if (!g_GlfwInited)
			{
				if (!glfwInit())
				{
					// TODO: log the problem
					return WindowHandle();
				}
				g_GlfwInited = true;
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
				glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			}

			GLFWmonitor* monitor = nullptr;
			if (window.FullScreen)
				monitor = glfwGetPrimaryMonitor();
			GLFWwindow* win = glfwCreateWindow(window.Width, window.Height, _title, monitor, g_FirstWindow);
			if (!win)
			{
				// TODO: log the problem
				return WindowHandle();
			}
			if (!g_FirstWindow)
			{
				g_FirstWindow = win;
				glfwMakeContextCurrent(win);
				flextInit(win);
			}

			uint32_t index;
			const uint32_t handle = core::HandleManager<WindowHandle>::GetNewHandle(index);
			a_assert(index < MaxWindow);
			g_Windows[index] = win;

			return { handle, index };
		}

		GLFWwindow* GetGlfwWindow(const WindowHandle& handle)
		{
			return g_Windows[handle.Index];
		}

		bool Run()
		{
			glfwPollEvents();
			return !glfwWindowShouldClose(g_FirstWindow);
		}
        
        core::RectI GetWindowSize(const WindowHandle& handle)
		{
			core::RectI v;
            glfwGetFramebufferSize(g_Windows[handle.Index], &v.width, &v.height);

			return v;
		}

    } // namespace io
    
} // namespace ari
