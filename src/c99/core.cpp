#include "core.h"
#include "sx/hash.h"
#include "sx/timer.h"
#include "sx/os.h"
#include "core/log.h"
#include <sx/allocator.h>
#include <sx/threads.h>
#include "core/memory/Memory.hpp"
#include "PoolAllocator.h"
#include "FreeListAllocator.h"

uint32_t HashStringFNV32(char* _str)
{
	return sx_hash_fnv32_str(_str);
}

void ari_os_sleep(int ms)
{
	sx_os_sleep(ms);
}

// Memory
void ARI_FREE_MEM(void* _ptr)
{
	ari::core::Memory::Free(_ptr);
}

// Pool allocator
void* CreatePoolAllocator(uint32_t totalSize, uint32_t chunkSize)
{
	return ari::core::Memory::New<PoolAllocator>(totalSize, chunkSize);
}

void DeletePoolAllocator(void* obj)
{
	ari::core::Memory::Delete(reinterpret_cast<PoolAllocator*>(obj));
}

void* PoolAllocate(void* obj, uint32_t size, uint32_t alignment)
{
	return reinterpret_cast<PoolAllocator*>(obj)->Allocate(size, alignment);
}

void PoolFree(void* obj, void* ptr)
{
	reinterpret_cast<PoolAllocator*>(obj)->Free(ptr);
}

void PoolInit(void* obj)
{
	reinterpret_cast<PoolAllocator*>(obj)->Init();
}

void PoolReset(void* obj)
{
	reinterpret_cast<PoolAllocator*>(obj)->Reset();
}

// Free list allocator
void* CreateFreeListAllocator(uint32_t totalSize, uint32_t pPolicy)
{
	return ari::core::Memory::New<FreeListAllocator>(totalSize, (FreeListAllocator::PlacementPolicy)pPolicy);
}

void DeleteFreeListAllocator(void* obj)
{
	ari::core::Memory::Delete(reinterpret_cast<FreeListAllocator*>(obj));
}

void* FreeListAllocate(void* obj, uint32_t size, uint32_t alignment)
{
	return reinterpret_cast<FreeListAllocator*>(obj)->Allocate(size, alignment);
}

void FreeListFree(void* obj, void* ptr)
{
	reinterpret_cast<FreeListAllocator*>(obj)->Free(ptr);
}

void FreeListInit(void* obj)
{
	reinterpret_cast<FreeListAllocator*>(obj)->Init();
}

void FreeListReset(void* obj)
{
	reinterpret_cast<FreeListAllocator*>(obj)->Reset();
}

// log functions
void ari_log_debug(char* _text)
{
	log_debug(_text);
}

void ari_log_error(char* _text)
{
	log_error(_text);
}

void ari_log_info(char* _text)
{
	log_info(_text);
}

void ari_log_warning(char* _text)
{
	log_warn(_text);
}

// Time functions
void ari_tm_init()
{
	sx_tm_init();
}

uint64_t ari_tm_now()
{
	return sx_tm_now();
}

uint64_t ari_tm_diff(uint64_t new_ticks, uint64_t old_ticks)
{
	return sx_tm_diff(new_ticks, old_ticks);
}

uint64_t ari_tm_since(uint64_t start_ticks)
{
	return sx_tm_since(start_ticks);
}

uint64_t ari_tm_laptime(uint64_t* last_ticks)
{
	return sx_tm_laptime(last_ticks);
}


double ari_tm_sec(uint64_t ticks)
{
	return sx_tm_sec(ticks);
}

double ari_tm_ms(uint64_t ticks)
{
	return sx_tm_ms(ticks);
}

double ari_tm_us(uint64_t ticks)
{
	return sx_tm_us(ticks);
}

double ari_tm_ns(uint64_t ticks)
{
	return sx_tm_ns(ticks);
}

// SPSC Queue
void* ari_queue_spsc_create(int item_sz, int capacity)
{
	return sx_queue_spsc_create(sx_alloc_malloc(), item_sz, capacity);
}

void ari_queue_spsc_destroy(void* queue)
{
	sx_queue_spsc_destroy((sx_queue_spsc*)queue, sx_alloc_malloc());
}

bool ari_queue_spsc_produce(void* queue, const void* data)
{
	return sx_queue_spsc_produce((sx_queue_spsc*)queue, data);
}

bool ari_queue_spsc_consume(void* queue, void* data)
{
	return sx_queue_spsc_consume((sx_queue_spsc*)queue, data);
}

bool ari_queue_spsc_grow(void* queue)
{
	return sx_queue_spsc_grow((sx_queue_spsc*)queue, sx_alloc_malloc());
}

bool ari_queue_spsc_full(const void* queue)
{
	return sx_queue_spsc_full((sx_queue_spsc*)queue);
}
