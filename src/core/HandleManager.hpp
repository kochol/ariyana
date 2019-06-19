#include <cstdint>
#include "containers/Map.hpp"
#include "containers/Queue.hpp"

namespace ari
{
    namespace core
    {
        struct HandleManager
        {
			
			uint32_t GetNewHandle(uint32_t& index);

			bool IsHandleValid(const uint32_t& handle) const;

			void RemoveHandle(const uint32_t& handle);

        private:

			Map<uint32_t, uint32_t> m_mHandleIndexMap;
			Queue<uint32_t>			m_qRemovedIndex;
			uint32_t				m_iLastIndex = 0;
        };
        
    } // namespace core
    
} // namespace ari
