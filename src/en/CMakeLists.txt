fips_begin_module(ari_en)
	fips_vs_warning_level(3)
	fips_deps(ari_core sx)
    fips_files(
        AriTypeId.hpp
        ComponentHandle.hpp
		ComponentTag.hpp
		Entity.hpp
		Node.hpp			Node.cpp
		System.hpp
		UpdateState.hpp
        World.hpp 			World.cpp
        WorldManager.hpp 	WorldManager.cpp
    )
fips_end_module()
