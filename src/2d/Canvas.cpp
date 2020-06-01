#include "Canvas.hpp"
#include "en/ComponentManager.hpp"
#include "en/World.hpp"
#include "gfx/gfx.hpp"
#include "Camera2D.hpp"

namespace ari::gfx
{
	extern GfxSetup g_setup;
}

namespace ari::en
{
    ARI_COMPONENT_IMP_CHILD(Canvas, Node2D)    
    
	void Canvas::UpdateRect()
	{
		auto r = gfx::GetViewportSize();
		float sx = float(this->Rect.width * r.width) / float(gfx::g_setup.window.Width);
		float sy = float(this->Rect.height * r.height) / float(gfx::g_setup.window.Height);
		sx = sx_min(sx, sy);
		_RealRect = this->Rect * sx;
		gfx::SetViewportSize(_RealRect);
		_finalMat[0] = sx_mat4_SRT(sx, sx, 1, 0, 0, 0, _RealRect.width / -2, _RealRect.height / -2, 0);
	}

	void Canvas::UpdateCamera(Camera2D* _camera, int _frameTurnIndex)
	{
		_camera->_finalMat[_frameTurnIndex] = _camera->_finalMat[_frameTurnIndex] * _finalMat[0];
	}

} // namespace ari::en
