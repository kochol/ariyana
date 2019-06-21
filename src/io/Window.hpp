#pragma once

#include "core/defines.hpp"

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

		WindowHandle CreateAriWindow(int _width, int _height, const char* _title);

        /**
         * @brief Run the windows and handle the events.
         * 
         * Need to call it every frame.
         * 
         * @return true when the application wants to continue
         * @return false when the user request to close the application
         */
		bool Run();
		
	} // namespace io
	
} // namespace ari
