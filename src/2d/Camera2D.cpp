#include "Camera2D.hpp"
#include "en/ComponentManager.hpp"
#include "gfx/gfx.hpp"

namespace ari::en
{
	ARI_COMPONENT_IMP_CHILD(Camera2D, Node2D);

	//------------------------------------------------------------------------------
	void Camera2D::MoveLR(const float& _speed)
	{
		Position.x += _speed;
	}

	//------------------------------------------------------------------------------
	void Camera2D::MoveUD(const float& _speed)
	{
		Position.y += _speed;
	}

	//------------------------------------------------------------------------------
	void Camera2D::Render(const int& _frameTurnIndex)
	{
		const core::RectI rect = gfx::GetViewportSize();
		_proj = sx_mat4_ortho(rect.width, rect.height, 0.1f, 100, 0, false);
		gfx::SetViewProjMatrix(_finalMat[_frameTurnIndex], _proj);
	}

	
}