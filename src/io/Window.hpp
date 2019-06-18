#pragma once

#include "core/Ptr.hpp"
#include "core/defines.hpp"

namespace ari
{
	namespace io
	{
		const int MaxWindow = 16;

		ARI_HANDLE(WindowHandle);
		
		struct Window
		{
			/* data */
		};

		WindowHandle CreateWindow(int _width, int _height, const char* _title);
		
	} // namespace io
	
} // namespace ari
