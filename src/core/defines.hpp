#include <cstdint>
#include <climits>

#define ARI_HANDLE(_name) \
struct _name \
{ \
    uint32_t Handle = UINT32_MAX; \
};
