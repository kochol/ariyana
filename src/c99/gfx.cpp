#include "gfx.h"
#include "gfx/gfx.hpp"
#include "gfx/Application.hpp"
#include "gfx/Mesh.hpp"

ari::Application* ari_main(int argc, char* argv[]) 
{ 
    return nullptr;
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