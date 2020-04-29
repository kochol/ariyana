#pragma once
#include <LinearAllocator.h>
#include <cstdint>
#include "core/containers/Array.hpp"

namespace ari::en
{
	class Node2D;
	class Camera2D;

	struct FrameData2D
	{
		core::Array<Node2D*> Nodes;
		uint32_t FrameNumber = 0;
		Camera2D* Camera2dObj;
		int FrameDataTurnIndex = 0;

	}; // FrameData2D

} // ari::en