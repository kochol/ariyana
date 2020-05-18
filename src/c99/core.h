#ifndef ARI_CORE_H
#define ARI_CORE_H

#include "macros.h"

struct Vector3
{
    float   x,
            y,
            z;
};

struct Quat
{
    float x,
        y,
        z,
        w;
};

CARI_API uint32_t HashStringFNV32(char* _str);

#endif // ARI_CORE_H
