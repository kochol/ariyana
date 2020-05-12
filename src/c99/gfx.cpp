#include "gfx.h"
#include "gfx/gfx.hpp"
#include "gfx/Application.hpp"

ari::Application* ari_main(int argc, char* argv[]) 
{ 
    return nullptr;
}

bool SetupGfx(GfxSetup& _setup)
{
    printf("SetupGfx: w:%d h:%d", _setup.window.Width, _setup.window.Height);
    union { GfxSetup c; ari::gfx::GfxSetup cpp; } setup = { _setup };
    return ari::gfx::SetupGfx(setup.cpp);
}
