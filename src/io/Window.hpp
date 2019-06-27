#pragma once

#include "core/defines.hpp"
#include "core/Rect.hpp"

struct GLFWwindow;

namespace ari
{
	namespace io
	{
		static const int MaxWindow = 16;

		ARI_HANDLE(WindowHandle);
		
		struct Window
		{
			int Width = 640;
			int Height = 480;
			bool FullScreen = false;
			bool HighDpi = false;
			float DpiScale = 1.0f;
		};        

		WindowHandle CreateAriWindow(Window& window, const char* _title);

		GLFWwindow* GetGlfwWindow(const WindowHandle& handle);

        /**
         * @brief Run the windows and handle the events.
         * 
         * Need to call it every frame.
         * 
         * @return true when the application wants to continue
         * @return false when the user request to close the application
         */
		bool Run();

		core::RectI GetWindowSize(const WindowHandle& handle);

	} // namespace io
	
} // namespace ari
