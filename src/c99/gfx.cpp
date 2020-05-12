#include "gfx.h"
#include "gfx/gfx.hpp"
#include "gfx/Application.hpp"

ari::Application* ari_main(int argc, char* argv[]) 
{ 
    return nullptr;
}

bool SetupGfx(GfxSetup& _setup)
{
    union { GfxSetup c; ari::gfx::GfxSetup cpp; } setup = { _setup };
    return ari::gfx::SetupGfx(setup.cpp);
}
