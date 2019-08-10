#pragma once

#include <cstdint>

namespace ari
{
	namespace en
	{
		ARI_HANDLE(ComponentHandleBase);

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
