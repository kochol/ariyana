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

CARI_API bool SetupGfx(GfxSetup& setup);

#endif // ARI_GFX_H
