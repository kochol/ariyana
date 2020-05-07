#include "BoxShape.hpp"
#include "gfx/Vertices.hpp"
#include "sokol_gfx.h"
#include "en/ComponentManager.hpp"

namespace ari::en
{
	ARI_COMPONENT_IMP(BoxShape)

	gfx::BufferHandle BoxShape::m_sVBPos;
	gfx::BufferHandle BoxShape::m_sVBColor;
	gfx::BufferHandle BoxShape::m_sVBTexcoord;
	gfx::BufferHandle BoxShape::m_sIB;
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
		0, 2, 1,  0, 3, 2,
		6, 4, 5,  7, 4, 6,
		8, 10, 9,  8, 11, 10,
		14, 12, 13,  15, 12, 14,
		16, 18, 17,  16, 19, 18,
		22, 20, 21,  23, 20, 22
	};

	BoxShape::BoxShape()
	{
		_isRenderable = true;
		SubMesh.Handle = core::HandleManager<gfx::SubMeshHandle>::GetNewHandle(SubMesh.Index);
		core::ObjectPool<gfx::SubMesh>::Setup(512);
		auto sub_mesh = core::ObjectPool<gfx::SubMesh>::New(SubMesh.Index);
		sub_mesh->IndexBuffer = m_sIB;
		sub_mesh->Type = gfx::PrimitiveType::Triangles;
		sub_mesh->AABB = sx_aabbwhd(2.f, 2.f, 2.f);
		sub_mesh->Position = m_sVBPos;
		sub_mesh->Texcoord = m_sVBTexcoord;
		sub_mesh->Color = m_sVBColor;
		sub_mesh->ElementsCount = 36;
	}

	void BoxShape::Render(const int& _frameTurnIndex)
	{		
		auto mvp = gfx::GetViewProjMatrix() * _finalMat[_frameTurnIndex];

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
		ApplyUniforms(gfx::ShaderStage::VertexShader, 0, mvp.f, sizeof(sx_mat4));
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
			gfx::PipelineSetup pipeline_setup;
			pipeline_setup.shader = gfx::GetShader(gfx::ShaderType::BasicVertexColor);
			pipeline_setup.layout.attrs[0].format = gfx::VertexFormat::Float3;
			pipeline_setup.layout.attrs[1].format = gfx::VertexFormat::UByte4N;
			pipeline_setup.layout.attrs[1].bufferIndex = 1;
			pipeline_setup.layout.attrs[2].format = gfx::VertexFormat::UByte4N;
			pipeline_setup.layout.attrs[2].bufferIndex = 2;
			pipeline_setup.index_type = gfx::IndexType::Uint16;

			m_sPipeline = gfx::CreatePipeline(pipeline_setup);

			gfx::Bindings bindings;
			bindings.vertexBuffers[0] = m_sVBPos;
			bindings.vertexBuffers[1] = m_sVBColor;
			bindings.vertexBuffers[2] = m_sVBColor;
			bindings.indexBuffer = m_sIB;

			m_sBinding = gfx::CreateBinding(bindings);

			// Create shader, pipeline and binding
			pipeline_setup.shader = gfx::GetShader(gfx::ShaderType::BasicTexture);
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
			DestroyPipeline(m_sPipeline);
			DestroyPipeline(m_sTexPipeline);
			DestroyBinding(m_sBinding);
			DestroyBinding(m_sTexBinding);
		}
	}
}
