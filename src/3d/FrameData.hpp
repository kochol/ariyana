#pragma once
#include <cstdint>
#include "core/containers/Array.hpp"

namespace ari::en
{
	class Node3D;
	class Camera;

	struct FrameData
	{
		core::Array<Node3D*> Nodes;
		uint32_t FrameNumber = 0;
		Camera* CameraObj;
		int FrameDataTurnIndex = 0;
		
	}; // FrameData

} // ari::en
