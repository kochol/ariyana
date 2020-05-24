#pragma once
#include <cstdint>
#include "containers/Map.hpp"
#include "containers/Queue.hpp"

namespace ari
{
    namespace core
    {
		template <typename T>
        struct HandleManager
        {		
			static uint32_t GetNewHandle(uint32_t& index)
			{				
				if (m_qRemovedIndex.Size() > 0)
				{
					m_qRemovedIndex.Dequeue(index);
				}
				else
				{
					index = m_iLastIndex++;
				}

				m_mHandleIndexMap.Add(m_iLastHandle, index);
				return m_iLastHandle++;
			}

			static uint32_t CreateHandleByIndex(const uint32_t& index)
			{
				m_mHandleIndexMap.Add(m_iLastHandle, index);
				return  m_iLastHandle++;
			}

			static uint32_t FindHandleByIndex(const uint32_t& index)
			{
				for (auto h : m_mHandleIndexMap)
					if (h.Value() == index)
						return h.Key();

				return aInvalidHandle;
			}

			static uint32_t FindIndex(const uint32_t& handle)
			{
				return m_mHandleIndexMap[handle];
			}

			static bool IsHandleValid(uint32_t& handle)
			{
				bool r = m_mHandleIndexMap.Contains(handle);
				if (!r)
					handle = ari::core::aInvalidHandle;
				return r;
			}

			static void RemoveHandle(const uint32_t& handle)
			{
				const int i = m_mHandleIndexMap.FindIndex(handle);
				a_assert(i != InvalidIndex);
				if (i != InvalidIndex)
				{
					uint32_t removed = m_mHandleIndexMap.ValueAtIndex(i);
					m_mHandleIndexMap.EraseIndex(i);
					m_qRemovedIndex.Enqueue(removed);
				}
				
			}

			static const Map<uint32_t, uint32_t>& GetHandles()
			{
				return m_mHandleIndexMap;
			}

        private:

			static Map<uint32_t, uint32_t>	m_mHandleIndexMap;
			static Queue<uint32_t>			m_qRemovedIndex;
			static uint32_t					m_iLastHandle;
			static uint32_t					m_iLastIndex;
        };

		template <class T>
		Map<uint32_t, uint32_t> HandleManager<T>::m_mHandleIndexMap;

		template <class T>
		Queue<uint32_t> HandleManager<T>::m_qRemovedIndex;

		template <class T>
		uint32_t HandleManager<T>::m_iLastHandle = 0;

		template <class T>
		uint32_t HandleManager<T>::m_iLastIndex = 0;
	
    } // namespace core
    
} // namespace ari
