#pragma once

#include "core/Ptr.hpp"
#include <cstdint>

namespace ari
{
	namespace io
	{
		const int MaxWindow = 16;
		
		struct Window
		{
			/* data */
		};

		core::Ptr<Window> CreateWindow(int _width, int _height, const char* _title);
		
	} // namespace io
	
} // namespace ari
