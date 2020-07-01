#pragma once

#include "UpdateState.hpp"
#include "core/containers/Array.hpp"

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

			virtual void Unconfigure(World* _world) { }

			virtual void Update(World* _world, const float& _elapsed,
				UpdateState::Enum _state) { }

			virtual bool NeedUpdateOn(UpdateState::Enum _state) { return false; }

			virtual void AddChildSystem(World* _world, System* pSystem) 
				{ m_Childs.Add(pSystem); pSystem->Configure(_world); }

		protected:

			core::Array<System*>	m_Childs;

		}; // System
		
	} // en
	
} // ari
