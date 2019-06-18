#include "LockScope.hpp"
namespace ari
{
    namespace core
    {
		LockScope::LockScope()
		{
			m_lock.Lock();
		}

		LockScope::~LockScope()
		{
			m_lock.UnLock();
		}

	} // namespace core
    
} // namespace ari
