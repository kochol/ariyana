#pragma once

#include "core/containers/Array.hpp"
#include "gfx.hpp"
#include "core/defines.hpp"
#include "gfx/Material.hpp"

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
		gfx::PipelineHandle				Pipeline;
		gfx::BindingHandle				Binding;
		gfx::Material					Material;
		sx_aabb							AABB = {{SX_FLOAT_MAX, SX_FLOAT_MAX, SX_FLOAT_MAX,
			-SX_FLOAT_MAX, -SX_FLOAT_MAX,	-SX_FLOAT_MAX}};
	};
	
} // namespace ari::gfx
