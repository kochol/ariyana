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
				index = m_iLastIndex;
				if (m_qRemovedIndex.Size() > 0)
					m_qRemovedIndex.Dequeue(index);

				m_mHandleIndexMap.Add(m_iLastIndex, index);
				return m_iLastIndex++;
			}

			static uint32_t GetHandleByIndex(const uint32_t& index)
			{
				for (auto h : m_mHandleIndexMap)
					if (h.Value() == index)
						return h.Key();

				return aInvalidHandle;
			}

			static bool IsHandleValid(const uint32_t& handle)
			{
				if (handle == aInvalidHandle)
					return false;
				return m_mHandleIndexMap.Contains(handle);
			}

			static void RemoveHandle(const uint32_t& handle)
			{
				const int i = m_mHandleIndexMap.FindIndex(handle);
				if (i != InvalidIndex)
				{
					uint32_t removed = m_mHandleIndexMap.ValueAtIndex(i);
					m_mHandleIndexMap.EraseIndex(i);
					m_qRemovedIndex.Enqueue(removed);
				}
			}

        private:

			static Map<uint32_t, uint32_t>	m_mHandleIndexMap;
			static Queue<uint32_t>			m_qRemovedIndex;
			static uint32_t					m_iLastIndex;
        };
        
    } // namespace core
    
} // namespace ari
