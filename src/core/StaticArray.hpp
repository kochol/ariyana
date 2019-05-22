#pragma once

namespace ari
{
	namespace core
	{
		template <typename T, int SIZE>
		struct StaticArray
		{
			T	m_Items[SIZE];
		};
		
	} // namespace core
	
} // namespace ari
