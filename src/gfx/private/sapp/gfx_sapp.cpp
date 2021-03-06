#include "sokol_gfx.h"
#if !defined ARI_NO_MAIN && !defined ARI_ANDROID
#define SOKOL_APP_IMPL
#define SOKOL_WIN32_FORCE_MAIN
#endif
#include "sokol_app.h"
#include "sokol_glue.h"
#include "core/memory/Memory.hpp"
#include "../../Application.hpp"
#include "gfx/gfx.hpp"
#include "io/FileSystem.hpp"
#include "sx/timer.h"

ari::Application* g_application = nullptr;
static uint64_t g_FrameNumber = 0;
static uint64_t last_time = 0;

#if !defined ARI_NO_MAIN && !defined ARI_ANDROID
struct sapp_data
{
	_sapp_t* p_sapp;
	_sapp_android_t* p_sapp_android_state;
};

extern "C" sapp_data CreateSg(sg_context_desc _desc)
{
	sg_desc desc;
	ari::core::Memory::Fill(&desc, sizeof(sg_desc), 0);
	desc.context = _desc;

	sg_setup(&desc);

	// Setup shaders
	ari::gfx::SetupShaders();

	return { &_sapp, &_sapp.android };
}

void ari_init_cb()
{
	sg_desc desc;
	ari::core::Memory::Fill(&desc, sizeof(sg_desc), 0);
	desc.context = sapp_sgcontext();

	sg_setup(&desc);
	sx_tm_init();
	last_time = sx_tm_now();

	// Setup shaders
	ari::gfx::SetupShaders();

	if (g_application)
		g_application->OnInit();
}

void ari_frame_cb()
{
    g_FrameNumber++;
	ari::io::Update();
	if (g_application)
	    g_application->OnFrame((float)sx_tm_sec(sx_tm_laptime(&last_time)));
}

void ari_cleanup_cb()
{
	if (g_application)
		g_application->OnCleanup();
	ari::core::Memory::Delete(g_application);
	sg_shutdown();
}

void ari_event_cb(const sapp_event* event)
{
	if (g_application)
		g_application->OnEvent((ari_event*)(event), {0, 0});
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
	desc.window_title = "Ari 0.8";

	desc.init_cb = ari_init_cb;
	desc.frame_cb = ari_frame_cb;
	desc.cleanup_cb = ari_cleanup_cb;
	desc.event_cb = ari_event_cb;
	desc.fail_cb = ari_fail_cb;

	return desc;
}
#endif

namespace ari
{
	namespace gfx
	{
		extern GfxSetup g_setup;

		bool SetupGfx(GfxSetup& setup)
		{
			g_setup = setup;
			return true;
		}

		void RenderToWindow(const io::WindowHandle& handle)
		{			
			a_assert(handle.Handle == 0); // Only use this with glfw backend.
		}

		void Present(const io::WindowHandle& handle)
		{
			a_assert(handle.Handle == 0); // Only use this with glfw backend.
		}

        uint64_t GetFrameNumber()
        {
            return g_FrameNumber;
        }

		uint64_t* GetFrameNumberPointer()
		{
			return &g_FrameNumber;
		}
	}

}
