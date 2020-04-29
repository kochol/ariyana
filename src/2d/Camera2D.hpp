#pragma once

#include "Node2D.hpp"
#include "gfx/Viewport.hpp"
#include "sx/math.h"

namespace ari::en
{
	enum class CameraType
	{
		Perspective,
		Orthographic
	};

	class Camera2D : public Node2D
	{
	public:
		ARI_COMPONENT_CHILD(Camera2D, Node2D)

		//! Constructor
		Camera2D()
		{
		}

		//! Destructor
		virtual ~Camera2D() = default;

		CameraType type;
		bool _isActive = false;

		//! Rotate the Camera around Z axis
		void Rotate(float _angle, const float& _zAxis);

		//! Move Camera left & right
		void MoveLR(const float& _speed);

		//! Move Camera up & down
		void MoveUD(const float& _speed);

		gfx::Viewport* GetViewport();

		void SetViewport(const gfx::Viewport& _viewport);



	protected:
		
		gfx::Viewport	m_viewport;
		bool			m_bHasViewPort = false;

	}; // Camera2D

} // ari::en
