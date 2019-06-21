#pragma once

#include "core/Ptr.hpp"
#include "core/defines.hpp"

namespace ari
{
	namespace io
	{
		static const int MaxWindow = 16;

		ARI_HANDLE(WindowHandle);
		
		struct Window
		{
			/* data */
		};

		WindowHandle CreateAriWindow(int _width, int _height, const char* _title);
		
	} // namespace io
	
} // namespace ari
