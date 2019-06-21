fips_begin_module(ari_io)
    fips_vs_warning_level(3)
    fips_deps(ari_core)
    fips_files(
        Window.hpp          Window.cpp
    )
    
    if (FIPS_WINDOWS)
        fips_dir(private/win)
        fips_files(
            WindowWin32.hpp WindowWin32.cpp
        )
    endif()
fips_end_module()
