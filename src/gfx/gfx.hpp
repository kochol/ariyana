#pragma once
#include "io/Window.hpp"

struct sg_shader_desc;

namespace ari
{
    namespace gfx
    {
		ARI_HANDLE(BufferHandle);
		ARI_HANDLE(ShaderHanlde);
		ARI_HANDLE(PipelineHandle);

        struct gfxSetup
        {
			io::Window	window;
			int sample_count = 0;                   /* MSAA sample count */
			int swap_interval = 0;                  /* the preferred swap interval (ignored on some platforms) */
		};

		enum class BufferUsage
		{
			Immutable = 1,
			Dynamic,
			Stream
		};

		bool SetupGfx(gfxSetup& setup);

		void Present();

		void Present(const io::WindowHandle& handle);

		BufferHandle CreateVertexBuffer(int size, void* content, BufferUsage usage = BufferUsage::Immutable);

		BufferHandle CreateIndexBuffer(int size, void* content, BufferUsage usage = BufferUsage::Immutable);

		ShaderHanlde CreateShader(const sg_shader_desc* desc);

    } // namespace gfx
    
} // namespace ari
