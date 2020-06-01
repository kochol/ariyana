#include "Sprite.hpp"
#include "gfx/Vertices.hpp"
#include "sokol_gfx.h"
#include "en/ComponentManager.hpp"
#include "sprite.glsl.h"

namespace ari::en
{
	ARI_COMPONENT_IMP_CHILD(Sprite, Node2D)
	
	gfx::BufferHandle Sprite::m_sVBPos;
	gfx::BufferHandle Sprite::m_sIB;
	gfx::PipelineHandle Sprite::m_sPipeline;
	gfx::BindingHandle Sprite::m_sBinding;

	static gfx::PosVertex2D s_SpritePosVertices[] =
	{
		{ -0.5f, -0.5f, 0.0f, 1.0f },	
		{ 0.5f, -0.5f, 1.0f, 1.0f },
		{ 0.5f, 0.5f, 1.0f, 0.0f },
		{ -0.5f, 0.5f, 0.0f, 0.0f },
	};


	static const uint16_t s_spriteTriList[] =
	{
		1,2,0, 3,0,2
	};

	Sprite::Sprite()
	{
		_isRenderable = true;

	}

	void Sprite::Render(const int& _frameTurnIndex)
	{
		ari_vs_params_t vs_params;
		vs_params.mvp = gfx::GetViewProjMatrix() * _finalMat[_frameTurnIndex];

		ari_fs_params_t fs_params;
		fs_params.color = v4Color;

		ApplyPipeline(m_sPipeline);

		if (Texture.IsValid())
		{
			SetTexture(m_sBinding, 0, Texture);
		}

		ApplyBindings(m_sBinding);

		ApplyUniforms(gfx::ShaderStage::VertexShader, SLOT_ari_vs_params, &vs_params, sizeof(ari_vs_params_t));
		ApplyUniforms(gfx::ShaderStage::FragmentShader , SLOT_ari_fs_params, &fs_params, sizeof(ari_fs_params_t));

		gfx::Draw(0, 6, 1);
	}

	
	void Sprite::Init(RenderSystem2D* render_system_2D)
	{
		if (m_sVBPos.Handle == core::aInvalidHandle)
		{
			// Create static vertex buffers.
			m_sVBPos = gfx::CreateVertexBuffer(sizeof(s_SpritePosVertices),
				(void*)s_SpritePosVertices);

		
			// Create static index buffer.
			m_sIB = gfx::CreateIndexBuffer(sizeof(s_spriteTriList),
				(void*)s_spriteTriList);

			// Create shader, pipeline and binding
			gfx::PipelineSetup pipeline_setup;
			pipeline_setup.shader = gfx::CreateShader(ari_sprite_shader_desc());
			// position
			pipeline_setup.layout.attrs[0].format = gfx::VertexFormat::Float2;
			// texCoord
			pipeline_setup.layout.attrs[1].format = gfx::VertexFormat::Float2;
			pipeline_setup.index_type = gfx::IndexType::Uint16;

			m_sPipeline = gfx::CreatePipeline(pipeline_setup);

			gfx::Bindings bindings;
			bindings.vertexBuffers[0] = m_sVBPos;
			bindings.indexBuffer = m_sIB;

			m_sBinding = gfx::CreateBinding(bindings);

		}
	}

	void Sprite::Shutdown()
	{
		if (m_sVBPos.Handle != core::aInvalidHandle)
		{
			DestroyBuffer(m_sVBPos);
			DestroyBuffer(m_sIB);
			DestroyPipeline(m_sPipeline);
			DestroyBinding(m_sBinding);
		}
	}

	void Sprite::SetColor(float r, float g, float b, float a)
	{
		v4Color.x = r;
		v4Color.y = g;
		v4Color.z = b;
		v4Color.w = a;
	}
} // ari::en
