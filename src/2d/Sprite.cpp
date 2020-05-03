#include "Sprite.hpp"
#include "gfx/Vertices.hpp"
#include "sokol_gfx.h"
#include "en/ComponentManager.hpp"

namespace ari::en
{
	ARI_COMPONENT_IMP(Sprite)

	static gfx::PosVertex2D s_SpritePosVertices[] =
	{
		{ -0.5f, -0.5f, -0.5f, -0.5f},
		{ 0.5f, -0.5f, 0.5f, -0.5f},
		{ 0.5f, 0.5f, 0.5f, 0.5f },
		{ -0.5f, 0.5f, -0.5f, 0.5f },
	};

	static const uint16_t s_spriteTriList[] =
	{
		0,2,1	3,0,2
	};

	Sprite::Sprite()
	{
		_isRenderable = true;
	}

	void Sprite::Render(const int& _frameTurnIndex)
	{
		// TODO: create sx_mat3 proj and view 
		auto mvp = gfx::GetViewProjMatrix() * _finalMat[_frameTurnIndex];

		if (Texture.IsValid())
		{
			//apply texture
		}
		else
		{
			// apply vertices
		}
	}

	
	void Sprite::Shutdown()
	{
		// Destroy buffers, pipelines and bindings
	}
} // ari::en