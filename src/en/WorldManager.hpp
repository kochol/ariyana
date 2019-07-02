#pragma once

namespace ari
{
	namespace en
	{
		struct World;

		struct WorldManager
		{
			//! Create a new World
			World* CreateWorld();

			//! Destroy a world
			void DestroyWorld(World* _world);

		};
		
	} // en
	
} // ari
