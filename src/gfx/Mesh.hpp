#pragma once

#include "SubMesh.hpp"

namespace ari::gfx
{
	ARI_HANDLE(MeshHandle)

	struct Mesh
	{
		core::Array<SubMeshHandle> SubMeshes;
	};

	Mesh* GetMesh(const MeshHandle& mesh_handle);
	
} // namespace ari::gfx
