#pragma once

#include "core/defines.hpp"
#include "core/containers/Array.hpp"

namespace ari::gfx
{
	ARI_HANDLE(SubMeshHandle)
	
	struct SubMesh
	{
		core::Array<gfx::BufferHandle>	VertexBuffers;
		gfx::BufferHandle				IndexBuffer;
	};
	
} // namespace ari::gfx
