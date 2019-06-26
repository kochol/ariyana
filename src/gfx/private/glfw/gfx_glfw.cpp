#include "gfx/gfx.hpp"
#include "io/private/flextgl/flextGL.h"
#define SOKOL_IMPL
#include "sokol_gfx.h"
#include "GLFW/glfw3.h"

namespace ari
{
    namespace gfx
    {
		bool ari::gfx::SetupGfx(gfxSetup& setup)
		{
			const io::WindowHandle window = io::CreateAriWindow(setup.window, "Ari 0.1");
			if (!window.IsValid())
				return false;

			// Setup sokol gfx
			sg_desc desc;
			core::Memory::Fill(&desc, sizeof(sg_desc), 0);
			sg_setup(&desc);

			return true;
		}

		void Present()
		{
			const io::WindowHandle window{ 0 , 0 };
			Present(window);
		}

		void Present(const io::WindowHandle& handle)
		{
			glfwSwapBuffers(io::GetGlfwWindow(handle));
		}

    } // namespace gfx
    
} // namespace ari
