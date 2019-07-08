#include "BoxShape.hpp"
#include "gfx/Vertices.hpp"
#include "sokol_gfx.h"
#include "Box.glsl.h"

namespace ari::en
{
	gfx::BufferHandle BoxShape::m_sVBPos;
	gfx::BufferHandle BoxShape::m_sVBColor;
	gfx::BufferHandle BoxShape::m_sIB;
	gfx::ShaderHanlde BoxShape::m_sProgram;
	gfx::PipelineHandle BoxShape::m_sPipeline;
	gfx::BindingHandle BoxShape::m_sBinding;

	static gfx::PosVertex s_cubePosVertices[] =
	{
		{ -1.0f,  1.0f,  1.0f },
		{ 1.0f,  1.0f,  1.0f },
		{ -1.0f, -1.0f,  1.0f },
		{ 1.0f, -1.0f,  1.0f },
		{ -1.0f,  1.0f, -1.0f },
		{ 1.0f,  1.0f, -1.0f },
		{ -1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
	};

	static gfx::ColorVertex s_cubeColorVertices[] =
	{
		{ 0xff000000 },
		{ 0xff0000ff },
		{ 0xff00ff00 },
		{ 0xff00ffff },
		{ 0xffff0000 },
		{ 0xffff00ff },
		{ 0xffffff00 },
		{ 0xffffffff },
	};

	static const uint16_t s_cubeTriList[] =
	{
		0, 1, 2, // 0
		1, 3, 2,
		4, 6, 5, // 2
		5, 6, 7,
		0, 2, 4, // 4
		4, 2, 6,
		1, 5, 3, // 6
		5, 7, 3,
		0, 4, 1, // 8
		4, 5, 1,
		2, 3, 6, // 10
		6, 3, 7,
	};

	void BoxShape::Render(const int& _frameTurnIndex)
	{		
		vs_params_t vs_params;
		vs_params.mvp = gfx::GetViewProjMatrix() * _finalMat[_frameTurnIndex];
		ApplyPipeline(m_sPipeline);
		ApplyBindings(m_sBinding);
		ApplyUniforms(gfx::ShaderStage::VertexShader, SLOT_vs_params, &vs_params, sizeof(vs_params));
		gfx::Draw(0, 36, 1);
	}

	void BoxShape::Init(RenderSystem * render_system)
	{
		if (m_sVBPos.Handle != core::aInvalidHandle)
		{
			// Create static vertex buffers.
			m_sVBPos = gfx::CreateVertexBuffer(sizeof(s_cubePosVertices),
				(void*)s_cubePosVertices);

			m_sVBColor = gfx::CreateVertexBuffer(sizeof(s_cubeColorVertices),
				(void*)s_cubeColorVertices);

			// Create static index buffer.
			m_sIB = gfx::CreateIndexBuffer(sizeof(s_cubeTriList),
				(void*)s_cubeTriList);

			m_sProgram = gfx::CreateShader(box_shader_desc());

			gfx::PipelineSetup pipeline_setup;
			pipeline_setup.shader = m_sProgram;
			pipeline_setup.layout.attrs[0].format = gfx::VertexFormat::Float3;
			pipeline_setup.layout.attrs[1].format = gfx::VertexFormat::Float4;
			pipeline_setup.layout.attrs[1].bufferIndex = 1;

			m_sPipeline = gfx::CreatePipeline(pipeline_setup);

			gfx::Bindings bindings;
			bindings.vertexBuffers[0] = m_sVBPos;
			bindings.vertexBuffers[1] = m_sVBColor;
			bindings.indexBuffer = m_sIB;

			m_sBinding = gfx::CreateBinding(bindings);
		}
	}

	void BoxShape::Shutdown()
	{
		if (m_sVBPos.Handle != core::aInvalidHandle)
		{
			DestroyBuffer(m_sVBPos);
			DestroyBuffer(m_sVBColor);
			DestroyBuffer(m_sIB);
			DestroyShader(m_sProgram);
			DestroyPipeline(m_sPipeline);
			DestroyBinding(m_sBinding);
		}
	}
}
