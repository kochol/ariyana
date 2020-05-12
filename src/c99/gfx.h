#ifndef ARI_GFX_H
#define ARI_GFX_H

#include <stdio.h>
#include <stdint.h>
#if defined _WIN32 || defined __CYGWIN__
    #ifdef CARI_NO_EXPORT
        #define API
    #else
        #define API __declspec(dllexport)
    #endif
#else
    #ifdef __GNUC__
        #define API  __attribute__((__visibility__("default")))
    #else
        #define API
    #endif
#endif

#if defined __cplusplus
    #define EXTERN extern "C"
#else
    #include <stdarg.h>
    #include <stdbool.h>
    #define EXTERN extern
#endif

#define CARI_API EXTERN API
#define CONST const

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
