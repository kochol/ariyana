#pragma once
//------------------------------------------------------------------------------
/**
    @file core/Config.hpp
    @brief global configuration defines
*/

#if _DEBUG
#define ARI_DEBUG (1)
#endif

// does the platform have std::thread and/or thread-local compiler support?
#if ARI_FORCE_NO_THREADS
#define ARI_HAS_THREADS (0)
#define ARI_COMPILER_HAS_THREADLOCAL (0)
#define ARI_THREADLOCAL_PTHREAD (0)
#elif ARI_EMSCRIPTEN
#define ARI_HAS_THREADS (0)
#define ARI_COMPILER_HAS_THREADLOCAL (0)
#define ARI_THREADLOCAL_PTHREAD (0)
#elif ARI_IOS
#define ARI_HAS_THREADS (1)
#define ARI_COMPILER_HAS_THREADLOCAL (0)
#define ARI_THREADLOCAL_PTHREAD (1)
#else
#define ARI_HAS_THREADS (1)
#define ARI_COMPILER_HAS_THREADLOCAL (1)
#define ARI_THREADLOCAL_PTHREAD (0)
#endif

// does the platform have std::atomic support?
#define ARI_HAS_ATOMIC (1)

// platform specific max-alignment
#if ARI_EMSCRIPTEN
#define ARI_MAX_PLATFORM_ALIGN (4)
#else
/// max memory alignment for this platform
#define ARI_MAX_PLATFORM_ALIGN (16)
#endif

/// memory debug fill pattern (byte)
#define ARI_MEMORY_DEBUG_BYTE (0xBB)
/// memory debug fill pattern (short)
#define ARI_MEMORY_DEBUG_SHORT (0xBBBB)
/// memory debug fill pattern (int)
#define ARI_MEMORY_DEBUG_INT (0xBBBBBBBB)

/// minimum grow size for dynamic container classes (num elements)
#define ARI_CONTAINER_DEFAULT_MIN_GROW (16)
/// maximum grow size for dynamic container classes (num elements)
#define ARI_CONTAINER_DEFAULT_MAX_GROW (1<<16)

#ifndef __GNUC__
#define __attribute__(x)
#endif

/// silence unused variable warning
#define ARI_UNUSED __attribute__((unused))

/// stringify helper
#define __ARI_stringify(x) #x
#define ARI_STRINGIFY(x) __ARI_stringify(x)
