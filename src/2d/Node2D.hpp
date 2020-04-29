#pragma once

#include "en/Node.hpp"
#include "sx/math.h"
#include "core/defines.hpp"

namespace ari::en
{
	class Node2D : public Node
	{
	public:

		ARI_COMPONENT(Node2D)

		//! Constructor
		Node2D() : _isRenderable(false)
		{
			Position = sx_vec2f(0.0f, 0.0f);
			Scale = sx_vec2f(0.0f, 0.0f);
			Rotation = sx_quat_ident();
		}

		//! Destructor
		virtual ~Node2D() = default;

		//! Render
		virtual void Render(const int& _frameTurnIndex) { sx_unused(_frameTurnIndex); }

		sx_vec2 Position;
		sx_quat Rotation;
		sx_vec2 Scale;

		sx_mat4 Transform;

		sx_mat4 _finalMat[3];
		bool _isRenderable;

	}; // Node2D

} // ari::en

#include <Meta.h>

namespace meta
{
	template<>
	inline auto registerMembers<ari::en::Node2D>()
	{
		return members(
			member("position", &ari::en::Node2D::Position),
			member("Rotation", &ari::en::Node2D::Rotation),
			member("Scale", &ari::en::Node2D::Scale)
		);
	}
}