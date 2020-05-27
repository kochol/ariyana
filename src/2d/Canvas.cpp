#include "Canvas.hpp"
#include "en/ComponentManager.hpp"

namespace ari::en
{
	ARI_COMPONENT_IMP(Canvas);

	void Canvas::SetViewportRect() 
	{
		Size.xmax = 200;
	}
} // ari::en
