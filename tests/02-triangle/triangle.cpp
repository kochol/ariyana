#include "io/Window.hpp"
#include "gfx/gfx.hpp"
#include "sokol_gfx.h"
#include "triangle.glsl.h"
#include "gfx/Application.hpp"

class TriangleApp : public ari::Application
{
public:

	~TriangleApp() = default;

	ari::gfx::GfxSetup* GetGfxSetup() override
	{
		return &m_setup;
	}

	void OnInit() override
	{
		/* a vertex buffer */
		const float vertices[] = {
			// positions            // colors
			 0.0f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f
		};
		vb = ari::gfx::CreateVertexBuffer(sizeof(vertices), (void*)vertices);

		shader = ari::gfx::CreateShader(triangle_shader_desc());

		pipeline_setup.shader = shader;
		pipeline_setup.layout.attrs[0].format = ari::gfx::VertexFormat::Float3;
		pipeline_setup.layout.attrs[1].format = ari::gfx::VertexFormat::Float4;

		pipeline = CreatePipeline(pipeline_setup);

		ari::gfx::Bindings binds;
		binds.vertexBuffers[0] = vb;
		binding = CreateBinding(binds);
	}

	void OnFrame(float _elapsedTime) override
	{
		ari::gfx::BeginDefaultPass();
		ApplyPipeline(pipeline);
		ApplyBindings(binding);
		ari::gfx::Draw(0, 3, 1);
		ari::gfx::EndPass();
		ari::gfx::Commit();
		ari::gfx::Present();
	}

	void OnCleanup() override
	{
		
	}

	void OnEvent(ari::io::ari_event* event, ari::io::WindowHandle _window) override
	{

	}

private:

	ari::gfx::GfxSetup			m_setup;
	ari::gfx::BufferHandle		vb;
	ari::gfx::ShaderHanlde		shader;
	ari::gfx::PipelineSetup		pipeline_setup;
	ari::gfx::PipelineHandle	pipeline;
	ari::gfx::BindingHandle		binding;

};

ARI_MAIN(TriangleApp)
