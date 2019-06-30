fips_begin_module(ari_core)
    fips_vs_warning_level(3)
    fips_deps(memory-allocators)
    fips_files(
        Assertion.hpp
        defines.hpp
        HandleManager.hpp
        HandmadeMath.h      HandmadeMath.cpp
        LockScope.hpp       LockScope.cpp
        Memory.hpp          Memory.cpp
        MemoryPool.hpp
        Ptr.hpp
        Rect.hpp
        SpinLock.hpp        SpinLock.cpp
        Types.hpp
    )

    fips_dir(containers)
    fips_files(
        Array.hpp
        elementBuffer.hpp
        KeyValuePair.hpp
        Map.hpp
        Queue.hpp
        Slice.hpp
        StaticArray.hpp
    )
fips_end_module()
