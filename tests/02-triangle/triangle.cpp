#include "io/Window.hpp"
#include "gfx/gfx.hpp"
#include "sokol_gfx.h"
#include "triangle.glsl.h"

int main(int argc, char* argv[])
{
	ari::gfx::gfxSetup setup;
	SetupGfx(setup);

	/* a vertex buffer */
	const float vertices[] = {
		// positions            // colors
		 0.0f,  0.5f, 0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f, 1.0f
	};
	ari::gfx::BufferHandle vb = ari::gfx::CreateVertexBuffer(sizeof(vertices), (void*)vertices);

	ari::gfx::ShaderHanlde shader = ari::gfx::CreateShader(triangle_shader_desc());

	ari::gfx::PipelineSetup pipeline_setup;
	pipeline_setup.shader = shader;
	pipeline_setup.layout.attrs[0].format = ari::gfx::VertexFormat::Float3;
	pipeline_setup.layout.attrs[1].format = ari::gfx::VertexFormat::Float4;

	ari::gfx::PipelineHandle pipeline = CreatePipeline(pipeline_setup);

	ari::gfx::Bindings binds;
	binds.vertexBuffers[0] = vb;
	ari::gfx::BindingHandle binding = CreateBinding(binds);

	while(ari::io::Run())
	{
		ari::gfx::BeginDefaultPass();
		ApplyPipeline(pipeline);
		ApplyBindings(binding);
		ari::gfx::Draw(0, 3, 1);
		ari::gfx::EndPass();
		ari::gfx::Commit();
		ari::gfx::Present();
	}
	return 0;
}
