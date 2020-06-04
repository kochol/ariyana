#pragma once

#include "Node2D.hpp"
#include "gfx/Viewport.hpp"
#include "sx/math.h"
#include "en/World.hpp"

namespace ari::en
{

	class Camera2D : public Node2D
	{
	public:
		ARI_COMPONENT_CHILD(Camera2D, Node2D)

		//! Constructor
		Camera2D()
		{
			_isRenderable = true;
		}

		//! Destructor
		virtual ~Camera2D() = default;

		

		float		width,
					height;
		sx_mat4		_view,
					_proj;
		bool		_isActive = false;

		//! Move Camera left & right
		void MoveLR(const float& _speed);

		//! Move Camera up & down
		void MoveUD(const float& _speed);

		void Render(const int& _frameTurnIndex) override;

	}; // Camera2D

} // ari::en
