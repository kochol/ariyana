#include "HandleManager.hpp"

namespace ari
{
    namespace core
    {
		uint32_t HandleManager::GetNewHandle(uint32_t& index)
		{
			m_mHandleIndexMap.Add(m_iLastIndex, m_iLastIndex);
			return m_iLastIndex++;
		}
	} // namespace core
    
} // namespace ari
