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

	
	while(ari::io::Run())
	{
		ari::gfx::Present();
	}
	return 0;
}
