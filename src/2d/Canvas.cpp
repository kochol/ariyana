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
		float sx = float(r.width) / float(gfx::g_setup.window.Width);
		float sy = float(r.height) / float(gfx::g_setup.window.Height);
		float s = sx_min(sx, sy);
		_RealRect = this->Rect * s;
		gfx::SetViewportSize(_RealRect);
		_finalMat[0] = sx_mat4_SRT(s, s, 1, 0, 0, 0, 0, 0, 0);
	}

	void Canvas::UpdateCamera(Camera2D* _camera, int _frameTurnIndex)
	{
		_camera->_finalMat[_frameTurnIndex] = _camera->_finalMat[_frameTurnIndex] * _finalMat[0];
		_camera->_finalMat[_frameTurnIndex].m14 *= _finalMat[0].m11;
		_camera->_finalMat[_frameTurnIndex].m24 *= _finalMat[0].m11;
	}

} // namespace ari::en
