#ifndef ARI_GFX_H
#define ARI_GFX_H

#include "macros.h"

struct WindowSetup
{
    int Width;
    int Height;
    bool FullScreen;
    bool HighDpi;
    float DpiScale;
};

struct GfxSetup
{
    WindowSetup window;
    int sample_count;                   /* MSAA sample count */
    int swap_interval;                  /* the preferred swap interval (ignored on some platforms) */
};

// Globals
CARI_API bool SetupGfx(GfxSetup& setup);
CARI_API uint32_t* GetFrameNumberPointer();

// Texture
CARI_HANDLE(TextureHandle)
CARI_API bool IsValidTexture(uint32_t& _handle);
CARI_API TextureHandle LoadTexture(char* _path);

// SubMesh
CARI_HANDLE(SubMeshHandle)
CARI_API bool IsValidSubMesh(uint32_t& _handle);

#endif // ARI_GFX_H
