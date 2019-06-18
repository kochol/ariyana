#include "SpinLock.hpp"
namespace ari
{
    namespace core
    {
		void SpinLock::Lock()
		{
			sx_lock(&m_lock);
		}

		void SpinLock::UnLock()
		{
			sx_unlock(&m_lock);
		}

		int SpinLock::TryLock()
		{
			return sx_trylock(&m_lock);
		}

	} // namespace core
    
} // namespace ari
