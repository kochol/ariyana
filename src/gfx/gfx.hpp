#pragma once
#include "io/Window.hpp"

namespace ari
{
    namespace gfx
    {
        struct gfxSetup
        {
			io::Window	window;
			int sample_count = 0;                   /* MSAA sample count */
			int swap_interval = 0;                  /* the preferred swap interval (ignored on some platforms) */
		};

		bool SetupGfx(gfxSetup& setup);

		void Present();

		void Present(const io::WindowHandle& handle);
        
    } // namespace gfx
    
} // namespace ari
