#pragma once

#include "UpdateState.hpp"

namespace ari
{
	namespace en
	{
		struct World;

		class System
		{
		public:

			virtual ~System() { }

			virtual void Configure(World* _world) { }

			virtual void UnConfigure(World* _world) { }

			virtual void Update(World* _world, float _elapsed,
				UpdateState::Enum _state) { }

			virtual bool NeedUpdateOn(UpdateState::Enum _state) { return false; }

		}; // System
		
	} // en
	
} // ari
