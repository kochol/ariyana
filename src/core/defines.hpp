#pragma once

#include <cstdint>
#include "Types.hpp"

#define ARI_HANDLE(_name) \
struct _name \
{ \
    uint32_t Handle = aInvalidHandle; \
};
