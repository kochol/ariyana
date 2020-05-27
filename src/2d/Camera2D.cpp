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


	void Camera2D::SetWindowRect(sx_rect& _rect)
	{
		WindowRect = _rect;
	}

	void Camera2D::SetViewportRect(int _x, int _y, int _width, int _height, bool _top_left)
	{
		sx_rect rect;
		rect.xmin = _x;
		rect.ymin = _y;
		rect.xmax = _width;
		rect.ymax = _height;

		gfx::SetViewportRect(rect, _top_left);

	}
	void Camera2D::SetViewportRect(sx_rect& _rect, bool _top_left)
	{
		gfx::SetViewportRect(_rect, _top_left);
	}

	void Camera2D::SetViewportRect(bool _top_left)
	{
		sx_rect vpRect;
		int width = WindowRect.xmax;
		int height = (int)(width / aspectRatio + 0.5f);
		if (height > WindowRect.ymax)
		{
			height = WindowRect.ymax;
			width = (int)(height * aspectRatio + 0.5f);
		}
		vpRect.xmin = (WindowRect.xmax / 2) - (width / 2);
		vpRect.ymin = (WindowRect.ymax / 2) - (height / 2);
		vpRect.xmax = width;
		vpRect.ymax = height;
		gfx::SetViewportRect(vpRect, _top_left);
	}

	sx_vec2 Camera2D::GetViewportScale()
	{
		sx_vec2 v2Scale;
		v2Scale.x = (float)((float)(WindowRect.xmax) / (float)width);
		v2Scale.y = (float)((float)(WindowRect.ymax) / (float)height);
		return v2Scale;

	}

	void Camera2D::SetScissorRect(int _x, int _y, int _width, int _height, bool _top_left)
	{
		sx_rect rect;
		rect.xmin = _x;
		rect.ymin = _y;
		rect.xmax = _width;
		rect.ymax = _height;

		gfx::SetScissorRect(rect, _top_left);
	}
	
}