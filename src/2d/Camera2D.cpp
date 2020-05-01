#include "Camera2D.hpp"
#include "en/ComponentManager.hpp"

namespace ari::en
{
	ARI_COMPONENT_IMP(Camera2D);

	void Camera2D::Rotate(float _angle)
	{

	} // Rotate

	void Camera2D::MoveLR(const float& _speed)
	{
		Position.x += _speed;
		Target.y += _speed;
	}
	void Camera2D::MoveUD(const float& _speed)
	{
		Position.y += _speed;
		Target.y += _speed;
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

	
}