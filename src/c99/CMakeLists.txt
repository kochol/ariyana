fips_begin_lib(cari_2d)
    add_definitions(-DARI_NO_MAIN=1 -DCARI_NO_EXPORT=1)
    fips_vs_warning_level(3)
    fips_deps(ari_2d)
    fips_files(
        2d.h                    2d.cpp
        macros.h
    )
fips_end_lib()

fips_begin_lib(cari_3d)
    add_definitions(-DARI_NO_MAIN=1 -DCARI_NO_EXPORT=1)
    fips_vs_warning_level(3)
    fips_deps(ari_3d)
    fips_files(
        3d.h                    3d.cpp
        macros.h
    )
fips_end_lib()

fips_begin_lib(cari_core)
    add_definitions(-DARI_NO_MAIN=1 -DCARI_NO_EXPORT=1)
    fips_vs_warning_level(3)
    fips_deps(ari_core)
    fips_files(
        core.h                  core.cpp
        macros.h
    )
fips_end_lib()

fips_begin_lib(cari_en)
    add_definitions(-DARI_NO_MAIN=1 -DCARI_NO_EXPORT=1)
    fips_vs_warning_level(3)
    fips_deps(ari_en)
    fips_files(
        en.h                    en.cpp
        macros.h
    )
fips_end_lib()

fips_begin_lib(cari_gfx)
    add_definitions(-DARI_NO_MAIN=1 -DCARI_NO_EXPORT=1)
    fips_vs_warning_level(3)
    fips_deps(ari_gfx)
    fips_files(
        gfx.h                   gfx.cpp
        macros.h
    )
fips_end_lib()

fips_begin_lib(cari_gui)
    add_definitions(-DARI_NO_MAIN=1 -DCARI_NO_EXPORT=1 -DCIMGUI_NO_EXPORT)
    fips_vs_warning_level(3)
    fips_deps(ari_gui)
    fips_files(
        gui.h                   gui.cpp
        macros.h
    )
fips_end_lib()

fips_begin_lib(cari_io)
    add_definitions(-DARI_NO_MAIN=1 -DCARI_NO_EXPORT=1)
    fips_vs_warning_level(3)
    fips_deps(ari_fs_local)
    fips_files(
        io.h                    io.cpp
        macros.h
        zip_file.hpp
    )
fips_end_lib()

fips_begin_lib(cari_net)
    add_definitions(-DARI_NO_MAIN=1 -DCARI_NO_EXPORT=1)
    fips_vs_warning_level(3)
    fips_deps(ari_net)
    fips_files(
        macros.h
        net.h                   net.cpp
        netSerializer.hpp
    )
fips_end_lib()

fips_begin_sharedlib(cari)
    remove_definitions(-DCIMGUI_NO_EXPORT)
    add_definitions(-DARI_NO_MAIN=1 -DCARI_NO_EXPORT=0)
    fips_vs_warning_level(3)
    fips_deps(ari_3d ari_2d ari_fs_local ari_net ari_gui)
    fips_files(
        2d.h                    2d.cpp
        3d.h                    3d.cpp
        core.h                  core.cpp
        en.h                    en.cpp
        gfx.h                   gfx.cpp
        gui.h                   gui.cpp
        io.h                    io.cpp
        macros.h
        net.h                   net.cpp
        netSerializer.hpp
        zip_file.hpp
    )
fips_end_sharedlib()
