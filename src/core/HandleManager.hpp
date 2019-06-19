#include <cstdint>
#include "containers/Map.hpp"

namespace ari
{
    namespace core
    {
        struct HandleManager
        {
			uint32_t GetNewHandle(uint32_t& index);

        private:

			Map<uint32_t, uint32_t> m_mHandleIndexMap;
			uint32_t				m_iLastIndex = 0;
        };
        
    } // namespace core
    
} // namespace ari
