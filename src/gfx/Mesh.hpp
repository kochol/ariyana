#pragma once

#include "SubMesh.hpp"

namespace ari::gfx
{
	ARI_HANDLE(MeshHandle)

	struct Mesh
	{
		core::Array<SubMeshHandle> SubMeshes;
	};
	
} // namespace ari::gfx
