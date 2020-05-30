#pragma once

#include "core/defines.hpp"
#include "core/containers/Array.hpp"
#include "ComponentHandle.hpp"

namespace ari
{
	namespace en
	{
		struct Entity;
		struct World;

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
			friend struct World;

			//! Should network replicates this entity?
			bool bReplicates = false;
			World* p_world = nullptr;

			~Entity();

			template<typename FUNC>
			void GetComponents(FUNC _func)
			{
				for (auto it = mComponents.begin(); it != mComponents.end(); ++it)
				{
					for (auto it_cmp = it->value.begin(); it_cmp != it->value.end(); ++it_cmp)
					{
						if (it_cmp->IsBased)
							continue;
						uint32_t h = it_cmp->handle;
						_func(it->key, h, it_cmp->cmp);
					}
				}
			}

		private:
			struct cmp_pair
			{
				uint32_t handle;
				uint32_t index;
				void* cmp;
				bool IsBased = false;
			};

			core::Map<uint32_t, // cmp ID
				core::Array<cmp_pair>> mComponents;

			template<typename T>
			void RemoveComponent(ComponentHandle<T>& _cmp)
			{
				auto& l = mComponents[T::Id];
				for (int i = 0; i < l.Size(); ++i)
				{
					if (l[i].handle == _cmp.Handle)
					{
						l.EraseSwap(i);
						_cmp.Owner = nullptr;
						return;
					}
				}
			}

        };        

	} // en
	
} // ari
