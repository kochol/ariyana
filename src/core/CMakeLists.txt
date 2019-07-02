fips_begin_module(ari_core)
    fips_vs_warning_level(3)
    fips_deps(memory-allocators)
    fips_files(
        Assertion.hpp
        defines.hpp
        HandleManager.hpp
        HandmadeMath.h      HandmadeMath.cpp
        LockScope.hpp       LockScope.cpp
        Ptr.hpp
        Rect.hpp
        SpinLock.hpp        SpinLock.cpp
        StringHash.hpp
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

    fips_dir(memory)
    fips_files(
        Memory.hpp          Memory.cpp
        MemoryPool.hpp
    )

fips_end_module()
