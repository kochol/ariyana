#include "io/Window.hpp"
#include "io/private/flextgl/flextGL.h"
#include "GLFW/glfw3.h"
#include "core/containers/StaticArray.hpp"
#include "io/IOEvents.hpp"
#include "gfx/Application.hpp"
#include <sokol_gfx.h>

extern ari::Application* g_application;
ari::core::StaticArray<sg_context, ari::io::MaxWindow> g_sg_contexts;

namespace ari
{
    namespace io
    {
		static bool g_GlfwInited = false;
		GLFWwindow* g_FirstWindow = nullptr;
		core::StaticArray<GLFWwindow*, MaxWindow>	g_Windows;

		//------------------------------------------------------------------------------
		WindowHandle get_window_handle(GLFWwindow* window)
		{
			WindowHandle window_handle;
			for (uint32_t i = 0; i < MaxWindow; i++)
			{
				if (g_Windows[i] == window)
				{
					window_handle.Index = i;
					window_handle.Handle = core::HandleManager<WindowHandle>::FindHandleByIndex(i);
				}
			}
			return window_handle;
		}

		//------------------------------------------------------------------------------
		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (!g_application)
				return;

			ari_event event;
			if (GLFW_PRESS == action || GLFW_REPEAT == action)
				event.type = ARI_EVENTTYPE_KEY_DOWN;
			else if (GLFW_RELEASE == action)
				event.type = ARI_EVENTTYPE_KEY_UP;

			event.key_code = ari_keycode(key);
			event.modifiers = mods;

			g_application->OnEvent(&event, get_window_handle(window));
		}

		//------------------------------------------------------------------------------
		void character_callback(GLFWwindow* window, unsigned int codepoint)
		{
			if (!g_application)
				return;

			ari_event event;
			event.type = ARI_EVENTTYPE_CHAR;
			event.char_code = codepoint;

			g_application->OnEvent(&event, get_window_handle(window));
		}

		//------------------------------------------------------------------------------
		ari_mousebutton get_mouse_button(GLFWwindow* win)
		{
			if (GLFW_PRESS == glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT))
				return ARI_MOUSEBUTTON_LEFT;
			if (GLFW_PRESS == glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT))
				return ARI_MOUSEBUTTON_RIGHT;
			if (GLFW_PRESS == glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE))
				return ARI_MOUSEBUTTON_MIDDLE;
			return ARI_MOUSEBUTTON_INVALID;
		}

		//------------------------------------------------------------------------------
		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
		{
			if (!g_application)
				return;

			ari_event event;
			event.type = ARI_EVENTTYPE_MOUSE_MOVE;
			event.mouse_x = float(xpos);
			event.mouse_y = float(ypos);
			event.mouse_button = get_mouse_button(window);

			g_application->OnEvent(&event, get_window_handle(window));
		}

		//------------------------------------------------------------------------------
		void cursor_enter_callback(GLFWwindow* window, int entered)
		{
			if (!g_application)
				return;

			ari_event event;

			if (entered)
			{
				// The cursor entered the content area of the window
				event.type = ARI_EVENTTYPE_MOUSE_ENTER;
			}
			else
			{
				// The cursor left the content area of the window
				event.type = ARI_EVENTTYPE_MOUSE_LEAVE;
			}
			g_application->OnEvent(&event, get_window_handle(window));
		}
		

		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
		{
			if (!g_application)
				return;

			ari_event event;

			if (GLFW_PRESS == action)
				event.type = ARI_EVENTTYPE_MOUSE_DOWN;
			else if (GLFW_RELEASE == action)
				event.type = ARI_EVENTTYPE_MOUSE_UP;

			if (button == GLFW_MOUSE_BUTTON_LEFT)
				event.mouse_button = ARI_MOUSEBUTTON_LEFT;
			else if (button == GLFW_MOUSE_BUTTON_RIGHT)
				event.mouse_button = ARI_MOUSEBUTTON_RIGHT;
			else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
				event.mouse_button = ARI_MOUSEBUTTON_MIDDLE;

			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos); 
			event.mouse_x = float(xpos);
			event.mouse_y = float(ypos);

			g_application->OnEvent(&event, get_window_handle(window));
		}

		//------------------------------------------------------------------------------
		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
		{
			if (!g_application)
				return;

			ari_event event;
			event.type = ARI_EVENTTYPE_MOUSE_SCROLL;
			event.scroll_x = float(xoffset);
			event.scroll_y = float(yoffset);

			g_application->OnEvent(&event, get_window_handle(window));
		}

		//------------------------------------------------------------------------------
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

			uint32_t index;
			const uint32_t handle = core::HandleManager<WindowHandle>::GetNewHandle(index);
			a_assert(index < MaxWindow);
			g_Windows[index] = win;

			if (!g_FirstWindow)
			{
				g_FirstWindow = win;
				glfwMakeContextCurrent(win);
				flextInit(win);
			}
			else
			{
				glfwMakeContextCurrent(win);
				g_sg_contexts[index] = sg_setup_context();
			}

			// Set the events callbacks
			glfwSetKeyCallback(win, key_callback);
			glfwSetCharCallback(win, character_callback);
			glfwSetCursorPosCallback(win, cursor_position_callback);
			glfwSetCursorEnterCallback(win, cursor_enter_callback);
			glfwSetMouseButtonCallback(win, mouse_button_callback);
			glfwSetScrollCallback(win, scroll_callback);
				
			return { handle, index };
		}

		//------------------------------------------------------------------------------
		GLFWwindow* GetGlfwWindow(const WindowHandle& handle)
		{
			return g_Windows[handle.Index];
		}

		//------------------------------------------------------------------------------
		bool Run()
		{
			glfwPollEvents();
			return !glfwWindowShouldClose(g_FirstWindow);
		}
        
		//------------------------------------------------------------------------------
        core::RectI GetWindowSize(const WindowHandle& handle)
		{
			core::RectI v;
            glfwGetFramebufferSize(g_Windows[handle.Index], &v.width, &v.height);

			return v;
		}

		//------------------------------------------------------------------------------
		const void* AndroidGetNativeActivity()
		{
			return nullptr;
		}

    } // namespace io
    
} // namespace ari
