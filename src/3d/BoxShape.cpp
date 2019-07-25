#include "BoxShape.hpp"
#include "gfx/Vertices.hpp"
#include "sokol_gfx.h"
#include "Box.glsl.h"

namespace ari::en
{
	gfx::BufferHandle BoxShape::m_sVBPos;
	gfx::BufferHandle BoxShape::m_sVBColor;
	gfx::BufferHandle BoxShape::m_sVBTexcoord;
	gfx::BufferHandle BoxShape::m_sIB;
	gfx::ShaderHanlde BoxShape::m_sProgram;
	gfx::ShaderHanlde BoxShape::m_sTexProgram;
	gfx::PipelineHandle BoxShape::m_sPipeline;
	gfx::PipelineHandle BoxShape::m_sTexPipeline;
	gfx::BindingHandle BoxShape::m_sBinding;
	gfx::BindingHandle BoxShape::m_sTexBinding;

	static gfx::PosVertex s_cubePosVertices[] =
	{
		{ -1.0f, -1.0f, -1.0f},
		{  1.0f, -1.0f, -1.0f},
		{  1.0f,  1.0f, -1.0f},
		{ -1.0f,  1.0f, -1.0f},

		{ -1.0f, -1.0f,  1.0f},
		{  1.0f, -1.0f,  1.0f},
		{  1.0f,  1.0f,  1.0f},
		{ -1.0f,  1.0f,  1.0f},

		{ -1.0f, -1.0f, -1.0f},
		{ -1.0f,  1.0f, -1.0f},
		{ -1.0f,  1.0f,  1.0f},
		{ -1.0f, -1.0f,  1.0f},

		{  1.0f, -1.0f, -1.0f},
		{  1.0f,  1.0f, -1.0f},
		{  1.0f,  1.0f,  1.0f},
		{  1.0f, -1.0f,  1.0f},

		{ -1.0f, -1.0f, -1.0f},
		{ -1.0f, -1.0f,  1.0f},
		{  1.0f, -1.0f,  1.0f},
		{  1.0f, -1.0f, -1.0f},

		{ -1.0f,  1.0f, -1.0f},
		{ -1.0f,  1.0f,  1.0f},
		{  1.0f,  1.0f,  1.0f},
		{  1.0f,  1.0f, -1.0f},
	};

	static sx_vec2 s_cubeCoords[] =
	{
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },

		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },

		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },

		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },

		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },

		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f },
	};

	static gfx::ColorVertex s_cubeColorVertices[] =
	{
		{0xFF0000FF},
		{0xFF0000FF},
		{0xFF0000FF},
		{0xFF0000FF},

		{0xFF00FF00},
		{0xFF00FF00},
		{0xFF00FF00},
		{0xFF00FF00},

		{0xFFFF0000},
		{0xFFFF0000},
		{0xFFFF0000},
		{0xFFFF0000},

		{0xFFFF007F},
		{0xFFFF007F},
		{0xFFFF007F},
		{0xFFFF007F},

		{0xFFFF7F00},
		{0xFFFF7F00},
		{0xFFFF7F00},
		{0xFFFF7F00},

		{0xFF007FFF},
		{0xFF007FFF},
		{0xFF007FFF},
		{0xFF007FFF},
	};

	static const uint16_t s_cubeTriList[] =
	{
		0, 1, 2,  0, 2, 3,
		6, 5, 4,  7, 6, 4,
		8, 9, 10,  8, 10, 11,
		14, 13, 12,  15, 14, 12,
		16, 17, 18,  16, 18, 19,
		22, 21, 20,  23, 22, 20
	};

	void BoxShape::Render(const int& _frameTurnIndex)
	{		
		vs_params_t vs_params;
		vs_params.mvp = gfx::GetViewProjMatrix() * _finalMat[_frameTurnIndex];

		if (Texture.IsValid())
		{
			ApplyPipeline(m_sTexPipeline);
			SetTexture(m_sTexBinding, 0, Texture);
			ApplyBindings(m_sTexBinding);
		}
		else
		{
			ApplyPipeline(m_sPipeline);
			ApplyBindings(m_sBinding);
		}
		ApplyUniforms(gfx::ShaderStage::VertexShader, SLOT_vs_params, &vs_params, sizeof(vs_params));
		gfx::Draw(0, 36, 1);
	}

	void BoxShape::Init(RenderSystem * render_system)
	{
		if (m_sVBPos.Handle == core::aInvalidHandle)
		{
			// Create static vertex buffers.
			m_sVBPos = gfx::CreateVertexBuffer(sizeof(s_cubePosVertices),
				(void*)s_cubePosVertices);

			m_sVBColor = gfx::CreateVertexBuffer(sizeof(s_cubeColorVertices),
				(void*)s_cubeColorVertices);

			m_sVBTexcoord = gfx::CreateVertexBuffer(sizeof(s_cubeCoords),
				(void*)s_cubeCoords);

			// Create static index buffer.
			m_sIB = gfx::CreateIndexBuffer(sizeof(s_cubeTriList),
				(void*)s_cubeTriList);

			// Create shader, pipeline and binding
			m_sProgram = gfx::CreateShader(box_shader_desc());

			gfx::PipelineSetup pipeline_setup;
			pipeline_setup.shader = m_sProgram;
			pipeline_setup.layout.attrs[0].format = gfx::VertexFormat::Float3;
			pipeline_setup.layout.attrs[1].format = gfx::VertexFormat::UByte4N;
			pipeline_setup.layout.attrs[1].bufferIndex = 1;
			pipeline_setup.index_type = gfx::IndexType::Uint16;			

			m_sPipeline = gfx::CreatePipeline(pipeline_setup);

			gfx::Bindings bindings;
			bindings.vertexBuffers[0] = m_sVBPos;
			bindings.vertexBuffers[1] = m_sVBColor;
			bindings.indexBuffer = m_sIB;

			m_sBinding = gfx::CreateBinding(bindings);

			// Create shader, pipeline and binding
			m_sTexProgram = gfx::CreateShader(box_tex_shader_desc());

			pipeline_setup.shader = m_sTexProgram;
			pipeline_setup.layout.attrs[1].format = gfx::VertexFormat::Float2;

			m_sTexPipeline = gfx::CreatePipeline(pipeline_setup);

			bindings.vertexBuffers[1] = m_sVBTexcoord;

			m_sTexBinding = gfx::CreateBinding(bindings);
		}
	}

	void BoxShape::Shutdown()
	{
		if (m_sVBPos.Handle != core::aInvalidHandle)
		{
			DestroyBuffer(m_sVBPos);
			DestroyBuffer(m_sVBColor);
			DestroyBuffer(m_sVBTexcoord);
			DestroyBuffer(m_sIB);
			DestroyShader(m_sProgram);
			DestroyShader(m_sTexProgram);
			DestroyPipeline(m_sPipeline);
			DestroyPipeline(m_sTexPipeline);
			DestroyBinding(m_sBinding);
			DestroyBinding(m_sTexBinding);
		}
	}
}
