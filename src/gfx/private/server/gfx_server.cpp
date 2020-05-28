#include "gfx/gfx.hpp"
#include "gfx/Application.hpp"
#include "io/FileSystem.hpp"
#include "sx/timer.h"

#ifndef NDEBUG
#define NDEBUG
#define NDEBUG_DEF
#endif
#define SOKOL_IMPL
#include "sokol_gfx.h"
#ifdef NDEBUG_DEF
#undef NDEBUG
#endif

static uint64_t g_FrameNumber = 0;

namespace ari
{
    namespace gfx
    {
		bool SetupGfx(GfxSetup& setup)
		{
			// Setup sokol gfx
			sg_desc desc;
			core::Memory::Fill(&desc, sizeof(sg_desc), 0);
			sg_setup(&desc);

			// Setup shaders
			SetupShaders();

			return true;
		}

		void RenderToWindow(const io::WindowHandle& handle)
		{
		}

		void Present(const io::WindowHandle& handle)
		{
		}

        uint64_t GetFrameNumber()
        {
            return g_FrameNumber;
        }

		uint64_t* GetFrameNumberPointer()
		{
			return &g_FrameNumber;
		}

    } // namespace gfx
    
} // namespace ari

ari::Application* g_application = nullptr;

#ifndef ARI_NO_MAIN
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
}
#endif // ARI_NO_MAIN
