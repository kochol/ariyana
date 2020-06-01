#pragma once

#include <cstdint>
#include "core/Types.hpp"
#include "core/HandleManager.hpp"

namespace ari
{
	namespace en
	{
		struct Entity;

		template<class T>
		class ComponentHandle
		{
		public:
			uint32_t			Handle = core::aInvalidHandle;
			uint32_t			Index = core::aInvalidHandle;
			
			T*					Component = nullptr;
			Entity*				Owner = nullptr;

			T* operator->() const
			{
				return Component;
			}

			bool IsValid();

		}; // ComponentHandle

		template<class From, class To>
		ComponentHandle<To> CastComponentHandle(ComponentHandle<From> cmp)
		{
			ComponentHandle<To> r;
			r.Handle = cmp.Handle;
			r.Index = cmp.Index;
			r.Component = reinterpret_cast<To*>(cmp.Component);
			r.Owner = cmp.Owner;
			return r;
		}

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
		
	} // en
	
} // ari
