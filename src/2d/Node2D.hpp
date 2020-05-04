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
			Scale = sx_vec2f(1.0f, 1.0f);
			Rotation = 0.0f;
		}

		//! Destructor
		virtual ~Node2D() = default;

		//! Render
		virtual void Render(const int& _frameTurnIndex) { sx_unused(_frameTurnIndex); }

		sx_vec2 Position;
		float	Rotation; // rotation around z-axis
		sx_vec2 Scale;
		bool has_mat;
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
			member("Position", &ari::en::Node2D::Position),
			member("Rotation", &ari::en::Node2D::Rotation),
			member("Scale", &ari::en::Node2D::Scale)
		);
	}
}