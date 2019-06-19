#include "HandleManager.hpp"

namespace ari
{
    namespace core
    {
		template <class T>
		Map<uint32_t, uint32_t> HandleManager<T>::m_mHandleIndexMap;

		template <class T>
		Queue<uint32_t> HandleManager<T>::m_qRemovedIndex;

		template <class T>
		uint32_t HandleManager<T>::m_iLastIndex = 0;

	} // namespace core
    
} // namespace ari
