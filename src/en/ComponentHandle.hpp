#pragma once

#include <cstdint>
#include "core/Types.hpp"

namespace ari
{
	namespace en
	{
		template<class T>
		class ComponentHandle
		{
		public:
			uint32_t			Handle = core::aInvalidHandle;
			uint32_t			Index = core::aInvalidHandle;
			
			T*					Component;

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
			return r;
		}
		
	} // en
	
} // ari
