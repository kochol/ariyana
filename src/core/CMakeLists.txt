fips_begin_module(ari_core)

    fips_vs_warning_level(3)
    if (FIPS_MSVC)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif()

    fips_deps(memory-allocators)
    fips_files(
        Assertion.hpp
        defines.hpp
        Delegate.hpp
        HandleManager.hpp
        LockScope.hpp           LockScope.cpp
        log.h                   log.c
        Ptr.hpp
        Rect.hpp
        SpinLock.hpp            SpinLock.cpp
        Types.hpp
    )

    fips_dir(containers)
    fips_files(
        Array.hpp
        elementBuffer.hpp
        HashSet.hpp
        KeyValuePair.hpp
        Map.hpp
        Queue.hpp
        Set.hpp
        Slice.hpp
        StaticArray.hpp
    )

    fips_dir(memory)
    fips_files(
        Memory.hpp              Memory.cpp
        MemoryPool.hpp
        ObjectPool.hpp
    )

    fips_dir(string)
    fips_files(
        String.hpp              String.cpp
        StringAtom.hpp          StringAtom.cpp
        StringAtomBuffer.hpp    StringAtomBuffer.cpp
        StringAtomTable.hpp     StringAtomTable.cpp
        StringHash.hpp
    )

    fips_dir(threading)
    fips_files(
        ThreadLocalData.hpp     ThreadLocalData.cpp
        ThreadLocalPtr.hpp
    )

fips_end_module()
