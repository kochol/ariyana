#pragma once

#include "Node2D.hpp"
#include "gfx/Viewport.hpp"
#include "sx/math.h"

namespace ari::en
{

	class Camera2D : public Node2D
	{
	public:
		ARI_COMPONENT_CHILD(Camera2D, Node2D)

		//! Constructor
		Camera2D()
		{
			width = 640;
			height = 480;
			aspectRatio = width / height;
		}

		//! Destructor
		virtual ~Camera2D() = default;

		float		width,
					height;
		float		aspectRatio;
		sx_mat4		_view,
					_proj;
		bool		_isActive = false;
		sx_rect		WindowRect;

		//! Rotate the Camera around Z axis
		void Rotate(float _angle);


		//! Move Camera left & right
		void MoveLR(const float& _speed);

		//! Move Camera up & down
		void MoveUD(const float& _speed);

		gfx::Viewport* GetViewport();

		void SetViewport(const gfx::Viewport& _viewport);

		void SetWindowRect(sx_rect& _rect);

		void SetViewportRect(int _x, int _y, int _width, int _height, bool _top_left);
		void SetViewportRect(sx_rect& _rect, bool _top_left);
		//! set current window size to viewport rect size 
		void SetViewportRect(bool _top_left);

		sx_vec2 GetViewportScale();

		void SetScissorRect(int _x, int _y, int _width, int _height, bool _top_left);

	protected:

		
		gfx::Viewport	m_viewport;
		bool			m_bHasViewPort = false;

	}; // Camera2D

} // ari::en
