#ifndef ARI_GFX_H
#define ARI_GFX_H

#include "macros.h"
#include "core.h"

// Texture
CARI_HANDLE(TextureHandle)
CARI_API bool IsValidTexture(uint32_t& _handle);
CARI_API TextureHandle LoadTexture(char* _path);

// SubMesh
CARI_HANDLE(SubMeshHandle)
CARI_API bool IsValidSubMesh(uint32_t& _handle);

#endif // ARI_GFX_H
