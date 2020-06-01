#ifndef ARI_CORE_H
#define ARI_CORE_H

#include "macros.h"
#include "sx/macros.h"

struct Vector2
{
    float   x,
            y;
};

struct Vector3
{
    float   x,
            y,
            z;
};

struct Quat
{
    float	x,
			y,
			z,
			w;
};

struct Color
{
	float	r,
			g,
			b,
			a;
};

CARI_API uint32_t HashStringFNV32(char* _str);

CARI_API void ari_os_sleep(int ms);

// Time functions
CARI_API void ari_tm_init();
CARI_API uint64_t ari_tm_now();
CARI_API uint64_t ari_tm_diff(uint64_t new_ticks, uint64_t old_ticks);
CARI_API uint64_t ari_tm_since(uint64_t start_ticks);
CARI_API uint64_t ari_tm_laptime(uint64_t* last_ticks);

CARI_API double ari_tm_sec(uint64_t ticks);
CARI_API double ari_tm_ms(uint64_t ticks);
CARI_API double ari_tm_us(uint64_t ticks);
CARI_API double ari_tm_ns(uint64_t ticks);

#endif // ARI_CORE_H
