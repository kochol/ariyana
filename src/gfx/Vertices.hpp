#pragma once
#include <cstdint>

namespace ari::gfx
{
	struct PosVertex
	{
		float x, y, z;		
	};

	struct PosVertex2D
	{
		float x, y;
		float u, v;
	};

	struct ColorVertex
	{
		uint32_t argb;
	};

} // ari::gfx
