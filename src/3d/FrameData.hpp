#pragma once
#include <LinearAllocator.h>
#include <cstdint>
#include "core/containers/Array.hpp"
#include "sx/math.h"

namespace ari::en
{
	class Node3D;
	class Camera;

	struct FrameData
	{
		core::Array<Node3D*> Nodes;
		uint32_t FrameNumber = 0;
		Camera* CameraObj;
		sx_vec3 CameraPos;
		int FrameDataTurnIndex = 0;
		
	}; // FrameData

} // ari::en
