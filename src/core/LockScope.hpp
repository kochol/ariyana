#pragma once
#include "SpinLock.hpp"

namespace ari
{
    namespace core
    {
		//! This is an atomic lock useful for the short time of locks for longer locks use mutexes.
        struct LockScope
        {
            LockScope(SpinLock* pLock);

            ~LockScope();

        private:

			SpinLock	*	m_plock;

        };
       
    } // namespace core
    
} // namespace ari

#define LOCKSCOPE static ari::core::SpinLock ari_lock; ari::core::LockScope ari_lock_scope(&ari_lock);
