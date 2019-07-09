#pragma once

#include "Node3D.hpp"
#include "gfx/Viewport.hpp"
#include "sx/math.h"

namespace ari::en
{
	class Camera: public Node3D
	{
	public:

		//! Constructor
		Camera()
		{
			Target = sx_vec3f(0.0f, 0.0f, 100.0f);
			Up = sx_vec3f(0.0f, 1.0f, 0.0f);
			Right = sx_vec3f(1.0f, 0.0f, 0.0f);
		}

		//! Destructor
		virtual ~Camera() = default;

		sx_vec3		Target,
					Up,
					Right;
		float		Fov = 60.0f;
		sx_mat4		_view,
					_proj;
		bool		_isActive = false;
		
		//! Rotate the camera around an axis.
		void Rotate(float _angle, const sx_vec3& _axis);

		//! Rotate by mouse movement
		void RotateByMouse(int _x, int _y, float _speed);

		//! Move back & forward
		void MoveBF(const float& _speed);

		//! Move left & right
		void MoveLR(const float& _speed);

		//! Move up & down
		void MoveUD(const float& _speed);

		gfx::Viewport* GetViewport();

		void SetViewport(const gfx::Viewport& _viewport);

	protected:

		float			m_fCurRotX  = 0.0f, // Current Rotation X
						m_fLastRotX = 0.0f;	// Last Rotation X
		gfx::Viewport	m_viewport;
		bool			m_bHasViewPort = false;

	}; // Camera
	
} // ari::en
