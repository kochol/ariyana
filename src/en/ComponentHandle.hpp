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
		
	} // en
	
} // ari
