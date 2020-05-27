#include "Camera2D.hpp"
#include "en/ComponentManager.hpp"
#include "gfx/gfx.hpp"
namespace ari::en
{
	ARI_COMPONENT_IMP(Camera2D);

	void Camera2D::Rotate(float _angle)
	{
		Rotation = _angle;		
	} // Rotate

	void Camera2D::MoveLR(const float& _speed)
	{
		Position.x += _speed;
	}
	void Camera2D::MoveUD(const float& _speed)
	{
		Position.y += _speed;
	}

	gfx::Viewport* Camera2D::GetViewport()
	{
		if (!m_bHasViewPort)
			return nullptr;
		return &m_viewport;
	}

	void Camera2D::SetViewport(const gfx::Viewport& _viewport)
	{
		m_bHasViewPort = true;
		m_viewport = _viewport;
	}

	void Camera2D::SetViewportRect(int x, int y, int width, int height, bool top_left)
	{
		sx_rect rect;
		rect.xmin = x;
		rect.ymin = y;
		rect.xmax = width;
		rect.ymax = height;

		gfx::SetViewportRect(rect, top_left);

	}

	void Camera2D::SetViewportRect(bool top_left)
	{
		gfx::SetViewportRect(top_left);
	}

	void Camera2D::SetScissorRect(int x, int y, int width, int height, bool top_left)
	{
		sx_rect rect;
		rect.xmin = x;
		rect.ymin = y;
		rect.xmax = width;
		rect.ymax = height;

		gfx::SetScissorRect(rect, top_left);

	}
	
}