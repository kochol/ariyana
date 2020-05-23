#pragma once

#include "core/defines.hpp"
#include "core/containers/Array.hpp"

namespace ari
{
	namespace en
	{
		struct Entity;

		struct EntityHandle
		{
			uint32_t Handle = ari::core::aInvalidHandle;
			uint32_t Index = ari::core::aInvalidHandle;
			Entity* entity = nullptr;

			Entity* operator -> () const
			{
				return this->entity;
			}

			bool IsValid() 
			{ 
				if (this->entity == nullptr || Handle == ari::core::aInvalidHandle || Index == ari::core::aInvalidHandle) 
					return false; 
				return ari::core::HandleManager<EntityHandle>::IsHandleValid(Handle);
			} 
		};
			
		struct Entity
        {
			//! Should network replicates this entity?
			bool bReplicates = false;

			core::Map<uint32_t, // cmp ID
				core::Array<uint32_t>> mComponents;
        };        

	} // en
	
} // ari
