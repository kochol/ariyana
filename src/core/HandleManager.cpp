#include "HandleManager.hpp"

namespace ari
{
    namespace core
    {
		//------------------------------------------------------------------------------
		uint32_t HandleManager::GetNewHandle(uint32_t& index)
		{
			index = m_iLastIndex;
			if (m_qRemovedIndex.Size() > 0)
				m_qRemovedIndex.Dequeue(index);

			m_mHandleIndexMap.Add(m_iLastIndex, index);
			return m_iLastIndex++;
		}

		//------------------------------------------------------------------------------
		bool HandleManager::IsHandleValid(const uint32_t& handle) const
		{
			return m_mHandleIndexMap.Contains(handle);
		}

		//------------------------------------------------------------------------------
		void HandleManager::RemoveHandle(const uint32_t& handle)
		{
			const int i = m_mHandleIndexMap.FindIndex(handle);
			if (i != InvalidIndex)
			{
				uint32_t removed = m_mHandleIndexMap.ValueAtIndex(i);
				m_mHandleIndexMap.EraseIndex(i);
				m_qRemovedIndex.Enqueue(removed);
			}
		}

	} // namespace core
    
} // namespace ari
