#pragma once

#include "core/defines.hpp"
#include "core/containers/Array.hpp"

namespace ari::gfx
{
	ARI_HANDLE(SubMeshHandle)

	struct SubMesh
	{
		gfx::BufferHandle				IndexBuffer;
		gfx::PrimitiveType				Type;
		gfx::BufferHandle				Position,
										Normal,
										Tangent,
										Texcoord,
										Color,
										Joints,
										Weights;
		int								ElementsCount;
	};
	
} // namespace ari::gfx
