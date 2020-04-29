#include "Camera2D.hpp"
#include "en/ComponentManager.hpp"

namespace ari::en
{
	ARI_COMPONENT_IMP(Camera2D);

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