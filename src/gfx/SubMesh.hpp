#pragma once

#include "core/containers/Array.hpp"
#include "gfx.hpp"
#include "core/defines.hpp"

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

	};
	
} // namespace ari::gfx
