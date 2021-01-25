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

struct RectI
{
	int x,
		y,
		width,
		height;
};

CARI_API uint32_t HashStringFNV32(char* _str);

CARI_API void ari_os_sleep(int ms);

// Memory
CARI_API void ARI_FREE_MEM(void* _ptr);

// Pool allocator
CARI_API void* CreatePoolAllocator(uint32_t totalSize, uint32_t chunkSize);
CARI_API void DeletePoolAllocator(void* obj);
CARI_API void* PoolAllocate(void* obj, uint32_t size, uint32_t alignment);
CARI_API void PoolFree(void* obj, void* ptr);
CARI_API void PoolInit(void* obj);
CARI_API void PoolReset(void* obj);

// Free list allocator
CARI_API void* CreateFreeListAllocator(uint32_t totalSize, uint32_t pPolicy); // 0: First, 1: Best
CARI_API void DeleteFreeListAllocator(void* obj);
CARI_API void* FreeListAllocate(void* obj, uint32_t size, uint32_t alignment);
CARI_API void FreeListFree(void* obj, void* ptr);
CARI_API void FreeListInit(void* obj);
CARI_API void FreeListReset(void* obj);


// log functions
CARI_API void ari_log_debug(char* _text);
CARI_API void ari_log_error(char* _text);
CARI_API void ari_log_info(char* _text);
CARI_API void ari_log_warning(char* _text);

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

// SPSC Queue
CARI_API void* ari_queue_spsc_create(int item_sz, int capacity);
CARI_API void ari_queue_spsc_destroy(void* queue);
CARI_API bool ari_queue_spsc_produce(void* queue, const void* data);
CARI_API bool ari_queue_spsc_consume(void* queue, void* data);
CARI_API bool ari_queue_spsc_grow(void* queue);
CARI_API bool ari_queue_spsc_full(const void* queue);


#endif // ARI_CORE_H
