#pragma once

#include "core/defines.hpp"
#include "core/containers/Array.hpp"
#include "ComponentHandle.hpp"
#include "Node.hpp"

namespace ari
{
	namespace en
	{
		struct Entity;
		struct World;		
			
		struct Entity : public Node
        {
			friend struct World;

			ARI_COMPONENT(Entity);

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
						_func(it->key, it_cmp->handle, it_cmp->cmp);
					}
				}
			}

			template<typename T, typename FUNC>
			void GetComponents(FUNC _func)
			{
				if (mComponents.Contains(T::Id))
				{
					auto& l = mComponents[T::Id];
					for (auto& cmp : l)
					{
						ComponentHandle<T> h = { cmp.handle, cmp.index, reinterpret_cast<T*>(cmp.cmp), this };
						_func(h);
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
						break;
					}
				}
				if (T::Id != _cmp.Component->GetBaseId())
				{
					l = mComponents[_cmp.Component->GetBaseId()];
					for (int i = 0; i < l.Size(); ++i)
					{
						if (l[i].handle == _cmp.Handle)
						{
							l.EraseSwap(i);
							break;
						}
					}
				}
			}

        };        

	} // en
	
} // ari
