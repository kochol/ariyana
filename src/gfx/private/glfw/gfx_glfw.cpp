#include "gfx/gfx.hpp"
#include "io/private/flextgl/flextGL.h"
#define SOKOL_IMPL
#include "sokol_gfx.h"
#include "GLFW/glfw3.h"
#include "gfx/Application.hpp"
#include "io/FileSystem.hpp"
#include "sx/timer.h"
#include "core/containers/StaticArray.hpp"

static uint32_t g_FrameNumber = 0;
extern ari::core::StaticArray<sg_context, ari::io::MaxWindow> g_sg_contexts;

namespace ari
{
    namespace gfx
    {
		bool SetupGfx(GfxSetup& setup)
		{
			const io::WindowHandle window = io::CreateAriWindow(setup.window, "Ari 0.2");
			if (!window.IsValid())
				return false;

			glfwSwapInterval(setup.swap_interval);

			// Setup sokol gfx
			sg_desc desc;
			core::Memory::Fill(&desc, sizeof(sg_desc), 0);
			sg_setup(&desc);
			g_sg_contexts[0] = sg_setup_context();
			return true;
		}

		void RenderToWindow(const io::WindowHandle& handle)
		{
			glfwMakeContextCurrent(io::GetGlfwWindow(handle));			
			sg_activate_context(g_sg_contexts[handle.Index]);
		}

		void Present(const io::WindowHandle& handle)
		{
			glfwSwapBuffers(io::GetGlfwWindow(handle));
		}

        uint32_t GetFrameNumber()
        {
            return g_FrameNumber;
        }

    } // namespace gfx
    
} // namespace ari

ari::Application* g_application = nullptr;

int main(int argc, char* argv[]) 
{
    // call ari main
	g_application = ari_main(argc, argv);

	// set the params
	auto setup = g_application->GetGfxSetup();

	sx_tm_init();
	uint64_t last_time = sx_tm_now();
	ari::gfx::SetupGfx(*setup);
    g_application->OnInit();

    while (ari::io::Run())
    {
		ari::io::Update();
        g_FrameNumber++;		
        g_application->OnFrame((float)sx_tm_sec(sx_tm_laptime(&last_time)));
    }

    g_application->OnCleanup();
	ari::core::Memory::Delete(g_application);
    sg_shutdown();
}