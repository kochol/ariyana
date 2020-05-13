#pragma once
#include "sx/threads.h"

namespace ari
{
    namespace core
    {
		//! This is an atomic lock useful for the short time of locks for longer locks use mutexes.
        struct SpinLock
        {
            void Lock();

            void UnLock();

            int TryLock();

        private:

			sx_lock_t m_lock = 0;

        };
        
    } // namespace core
    
} // namespace ari
