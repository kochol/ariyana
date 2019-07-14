#pragma once
//------------------------------------------------------------------------------
/**
    @class ari::core::ThreadLocalData
    @brief manage a thread-local pointer table via pthread keys
    
    ThreadLocalData and ThreadLocalPtr enable thread-local support
    on platforms where the compiler doesn't have a thread-local keyword
    (only on iOS so far).
*/
#include "core/Types.hpp"
#if ARI_THREADLOCAL_PTHREAD
#include "core/Assertion.hpp"
#include <atomic>
#include <pthread.h>

namespace ari::core {
    
class ThreadLocalData {
public:
    /// setup once from main-thread
    static void SetupOnce();
    
    /// allocate a new slot
    static int Alloc();
    /// associate slot index with a pointer
    static void Set(int slotIndex, void* ptr);
    /// get pointer associated with slot
    static void* Get(int slotIndex);
    
private:
    /// get pointer to thread-local pointer-table
    static void** PointerTable();
    
    static const int MaxNumSlots = 1024;
    static pthread_key_t key;
    static std::atomic<int> curSlot;
};

//------------------------------------------------------------------------------
inline void*
ThreadLocalData::Get(int slotIndex) {
    a_assert_range_dbg(slotIndex, MaxNumSlots);
    void** table = PointerTable();
    return table[slotIndex];
}
    
} // namespace ari::core
#endif // ARI_COMPILER_HAS_THREADLOCAL
