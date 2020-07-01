#include "gfx.h"
#include "gfx/gfx.hpp"
#include "gfx/Application.hpp"
#include "gfx/Mesh.hpp"
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS 1
#include "cimgui/cimgui.h"

ari::Application* ari_main(int argc, char* argv[]) 
{ 
    // TODO: Move this function from here
    igIsAnyItemActive();
    return nullptr;
}

// Globals
bool SetupGfx(GfxSetup& _setup)
{
    union { GfxSetup c; ari::gfx::GfxSetup cpp; } setup = { _setup };
    return ari::gfx::SetupGfx(setup.cpp);
}

uint64_t* GetFrameNumberPointer()
{
    return ari::gfx::GetFrameNumberPointer();
}

void SetClearColor(Quat* _quat)
{
    ari::gfx::SetClearColor(*(sx_vec4*)_quat);
}

// Texture
bool IsValidTexture(uint32_t& _handle)
{
    return ari::core::HandleManager<ari::gfx::TextureHandle>::IsHandleValid(_handle);
}

TextureHandle LoadTexture(char* _path)
{
    union { ari::gfx::TextureHandle cpp; TextureHandle c{}; } r = { ari::gfx::LoadTexture(_path) };
    return r.c;
}

// SubMesh
bool IsValidSubMesh(uint32_t& _handle)
{
    return ari::core::HandleManager<ari::gfx::SubMeshHandle>::IsHandleValid(_handle);
}