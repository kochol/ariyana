#include "WorldManager.hpp"
#include "World.hpp"
#include <core/Memory.hpp>

namespace ari
{
	namespace en
	{
		//! Create a new World
		World* WorldManager::CreateWorld()
		{
			return core::Memory::New<World>();
		}

		//! Destroy a world
		void WorldManager::DestroyWorld(World* _world)
		{
			core::Memory::Delete<World>(_world);
		}

	} // en
	
} // ari
