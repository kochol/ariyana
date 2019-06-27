#define SOKOL_IMPL
#define SOKOL_WIN32_FORCE_MAIN
#include "sokol_app.h"
#define SOKOL_IMPL
#include "sokol_gfx.h"
#include "core/Memory.hpp"
#include "../../Application.hpp"
#include "gfx/gfx.hpp"

ari::Application* g_application = nullptr;

void ari_init_cb()
{
	sg_desc desc;
	ari::core::Memory::Fill(&desc, sizeof(sg_desc), 0);
	desc.mtl_device = sapp_metal_get_device();
	desc.mtl_renderpass_descriptor_cb = sapp_metal_get_renderpass_descriptor;
	desc.mtl_drawable_cb = sapp_metal_get_drawable;
	desc.d3d11_device = sapp_d3d11_get_device();
	desc.d3d11_device_context = sapp_d3d11_get_device_context();
	desc.d3d11_render_target_view_cb = sapp_d3d11_get_render_target_view;
	desc.d3d11_depth_stencil_view_cb = sapp_d3d11_get_depth_stencil_view;

	sg_setup(&desc);
	g_application->OnInit();
}

void ari_frame_cb()
{
	g_application->OnFrame();
}

void ari_cleanup_cb()
{
	g_application->OnCleanup();
	sg_shutdown();
	ari::core::Memory::Delete(g_application);
}

void ari_event_cb(const sapp_event* event)
{
	
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

		void Present()
		{
		}

		void Present(const io::WindowHandle& handle)
		{
		}

	}

}
