fips_begin_module(ari_core)
	fips_vs_warning_level(3)
    fips_files(
        Assertion.hpp
        defines.hpp
        LockScope.hpp       LockScope.cpp
        Memory.hpp          Memory.cpp
        Ptr.hpp
        SpinLock.hpp        SpinLock.cpp
        StaticArray.hpp
    )
fips_end_module()
