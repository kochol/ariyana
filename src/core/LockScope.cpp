#include "LockScope.hpp"
namespace ari
{
    namespace core
    {
		LockScope::LockScope(SpinLock* pLock)
		{
			m_plock = pLock;
			m_plock->Lock();
		}

		LockScope::~LockScope()
		{
			m_plock->UnLock();
		}

	} // namespace core
    
} // namespace ari
