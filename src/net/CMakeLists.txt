fips_begin_module(ari_net)
    set(CMAKE_CXX_STANDARD 17)
    ari_setup()
    fips_vs_warning_level(3)
    fips_deps(ari_en)
    fips_files(
        ClientSystem.hpp            ClientSystem.cpp
        GameAdapter.hpp             GameAdapter.cpp
        Messages.hpp                Messages.cpp
        net.hpp                     net.cpp
        NetworkSystem.hpp			NetworkSystem.cpp
        PropertyReplicator.hpp      PropertyReplicator.cpp
        RPC.hpp
        Serialize.hpp               Serialize.inl
        ServerSystem.hpp            ServerSystem.cpp
    )
fips_end_module()
