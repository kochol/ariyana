fips_begin_module(ari_en)
    set(CMAKE_CXX_STANDARD 17)
    ari_setup()
	fips_vs_warning_level(3)
    fips_libs(yojimbo)
	fips_deps(ari_core sx)
    fips_files(
        AriTypeId.hpp
        ComponentHandle.hpp
        ComponentManager.hpp
        Entity.hpp          Entity.cpp
        EventSubscriber.hpp
		Node.hpp			Node.cpp
		System.hpp
		UpdateState.hpp
        World.hpp 			World.cpp
        WorldManager.hpp 	WorldManager.cpp
    )
fips_end_module()
