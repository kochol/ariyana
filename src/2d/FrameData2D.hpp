#pragma once

#include <cstdint>
#include "core/containers/Array.hpp"

namespace ari::en
{
	class Node2D;
	class Camera2D;

	struct PassData2D
	{
		core::Array<Node2D*>	Nodes;
		Camera2D* camera_2d;
	};	

	struct FrameData2D
	{
		core::Array<Node2D*> Nodes;
		core::Array<PassData2D> Passes;
		uint32_t FrameNumber = 0;
		int FrameDataTurnIndex = 0;

	}; // FrameData2D

} // ari::en