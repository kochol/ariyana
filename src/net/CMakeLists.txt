fips_begin_module(ari_net)
    fips_vs_warning_level(3)
    fips_libs(yojimbo)
    fips_files(
        ClientSystem.hpp            ClientSystem.cpp
        GameAdapter.hpp             GameAdapter.cpp
        Messages.hpp
        net.hpp                     net.cpp
        ServerSystem.hpp            ServerSystem.cpp
    )
fips_end_module()
