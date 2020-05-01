#include "Sprite.hpp"
#include "gfx/Vertices.hpp"
#include "sokol_gfx.h"
#include "en/ComponentManager.hpp"

namespace ari::en
{
	ARI_COMPONENT_IMP(Sprite)

	static gfx::PosVertex2D s_SpritePosVertices[] =
	{
		{-1.0f, -1.0f},
		{1.0f, -1.0f},
		{1.0f, 1.0f},
		{-1.0f, 1.0f},
	};

	static const uint16_t s_spriteTriList[] =
	{
		0,2,1	3,0,2
	};

	Sprite::Sprite()
	{
		_isRenderable = true;
	}
	
	void Sprite::Shutdown()
	{
		// Destroy buffers, pipelines and bindings
	}
} // ari::en