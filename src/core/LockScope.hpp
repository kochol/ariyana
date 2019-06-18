#include "SpinLock.hpp"

namespace ari
{
    namespace core
    {
		//! This is an atomic lock useful for the short time of locks for longer locks use mutexes.
        struct LockScope
        {
            LockScope();

            ~LockScope();

        private:

			SpinLock	m_lock;

        };
        
    } // namespace core
    
} // namespace ari
