#define SOKOL_IMPL
#define SOKOL_WIN32_FORCE_MAIN
#include "sokol_app.h"
#define SOKOL_IMPL
#include "sokol_gfx.h"
#include "core/memory/Memory.hpp"
#include "../../Application.hpp"
#include "gfx/gfx.hpp"
#include "io/FileSystem.hpp"
#include "sx/timer.h"

ari::Application* g_application = nullptr;
static uint32_t g_FrameNumber = 0;
static uint64_t last_time = 0;

void ari_init_cb()
{
	sg_desc desc;
	ari::core::Memory::Fill(&desc, sizeof(sg_desc), 0);
	desc.gl_force_gles2 = sapp_gles2();
	desc.mtl_device = sapp_metal_get_device();
	desc.mtl_renderpass_descriptor_cb = sapp_metal_get_renderpass_descriptor;
	desc.mtl_drawable_cb = sapp_metal_get_drawable;
	desc.d3d11_device = sapp_d3d11_get_device();
	desc.d3d11_device_context = sapp_d3d11_get_device_context();
	desc.d3d11_render_target_view_cb = sapp_d3d11_get_render_target_view;
	desc.d3d11_depth_stencil_view_cb = sapp_d3d11_get_depth_stencil_view;

	sg_setup(&desc);
	sx_tm_init();
	last_time = sx_tm_now();
	g_application->OnInit();
}

void ari_frame_cb()
{
    g_FrameNumber++;
	ari::io::Update();
    g_application->OnFrame((float)sx_tm_sec(sx_tm_laptime(&last_time)));
}

void ari_cleanup_cb()
{
	g_application->OnCleanup();
	ari::core::Memory::Delete(g_application);
	sg_shutdown();
}

void ari_event_cb(const sapp_event* event)
{
	ari::io::ari_event eve;
	ari::core::Memory::Copy(event, &eve, sizeof(sapp_event));
	g_application->OnEvent(&eve, {0, 0});
}

void ari_fail_cb(const char* msg)
{
	
}

sapp_desc sokol_main(int argc, char* argv[]) {
	sapp_desc desc;
	ari::core::Memory::Fill(&desc, sizeof(sapp_desc), 0);

	// call ari main
	g_application = ari_main(argc, argv);

	// set the params
	const auto setup = g_application->GetGfxSetup();
	desc.width = setup->window.Width;
	desc.height = setup->window.Height;
	desc.fullscreen = setup->window.FullScreen;
	desc.high_dpi = setup->window.HighDpi;
	desc.sample_count = setup->sample_count;
	desc.swap_interval = setup->swap_interval;

	desc.init_cb = ari_init_cb;
	desc.frame_cb = ari_frame_cb;
	desc.cleanup_cb = ari_cleanup_cb;
	desc.event_cb = ari_event_cb;
	desc.fail_cb = ari_fail_cb;

	return desc;
}

namespace ari
{
	namespace gfx
	{
		bool SetupGfx(GfxSetup& setup)
		{
			return true;
		}

		void RenderToWindow(const io::WindowHandle& handle)
		{			
			a_assert(handle.Handle == 0); // Only use this with glfw backend.
		}

		void Present()
		{
		}

		void Present(const io::WindowHandle& handle)
		{
		}

        uint32_t GetFrameNumber()
        {
            return g_FrameNumber;
        }

	}

}
