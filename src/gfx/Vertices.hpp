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
		float tx, ty;
	};

	struct ColorVertex
	{
		uint32_t argb;
	};

} // ari::gfx
