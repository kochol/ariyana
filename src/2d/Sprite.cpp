#include "Sprite.hpp"
#include "gfx/Vertices.hpp"
#include "sokol_gfx.h"
#include "en/ComponentManager.hpp"

namespace ari::en
{
	ARI_COMPONENT_IMP(Sprite)
	
	gfx::BufferHandle Sprite::m_sVBPos;
	gfx::BufferHandle Sprite::m_sIB;
	gfx::PipelineHandle Sprite::m_sPipeline;
	gfx::PipelineHandle Sprite::m_sTexPipeline;
	gfx::BindingHandle Sprite::m_sBinding;
	gfx::BindingHandle Sprite::m_sTexBinding;

	static gfx::PosVertex2D s_SpritePosVertices[] =
	{
		{ -0.5f, -0.5f, 0.0f, 0.0f },
		{ 0.5f, -0.5f, 1.0f, 0.0f },
		{ 0.5f, 0.5f, 1.0f, 1.0f },
		{ -0.5f, 0.5f, 0.0f, 1.0f },
	};


	static const uint16_t s_spriteTriList[] =
	{
		0,2,1,	3,0,2
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
			ApplyPipeline(m_sTexPipeline);
			SetTexture(m_sTexBinding, 0, Texture);
			ApplyBindings(m_sTexBinding);
		}
		else
		{
			// apply vertices
			ApplyPipeline(m_sPipeline);
			ApplyBindings(m_sTexBinding);
		}
		ApplyUniforms(gfx::ShaderStage::VertexShader, 0, mvp.f, sizeof(sx_mat4));
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

			// Create shader, pipline and binding
			gfx::PipelineSetup pipeline_setup;
			pipeline_setup.shader = gfx::GetShader(gfx::ShaderType::BasicVertexColor);
			// position
			pipeline_setup.layout.attrs[0].format = gfx::VertexFormat::Float2;
			

			m_sPipeline = gfx::CreatePipeline(pipeline_setup);

			gfx::Bindings bindings;
			bindings.vertexBuffers[0] = m_sVBPos;
			bindings.indexBuffer = m_sIB;

			m_sBinding = gfx::CreateBinding(bindings);

			// Create shader, pipeline and binding
			pipeline_setup.shader = gfx::GetShader(gfx::ShaderType::BasicTexture);
			// texCoord
			pipeline_setup.layout.attrs[1].bufferIndex = 1;
			pipeline_setup.layout.attrs[1].offset = 2 * sizeof(float);
			pipeline_setup.layout.attrs[1].format = gfx::VertexFormat::Float2;

			m_sTexPipeline = gfx::CreatePipeline(pipeline_setup);

			bindings.vertexBuffers[0] = m_sVBPos;

			m_sTexBinding = gfx::CreateBinding(bindings);
		}
	}

	void Sprite::Shutdown()
	{
		if (m_sVBPos.Handle != core::aInvalidHandle)
		{
			DestroyBuffer(m_sVBPos);
			DestroyBuffer(m_sIB);
			DestroyPipeline(m_sPipeline);
			DestroyPipeline(m_sTexPipeline);
			DestroyBinding(m_sBinding);
			DestroyBinding(m_sTexBinding);
		}
	}
} // ari::en