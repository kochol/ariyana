#pragma once
#include <cstdint>

namespace ari::gfx
{
	struct PosVertex
	{
		float x, y, z;		
	};

	struct ColorVertex
	{
		uint32_t argb;
	};

} // ari::gfx
