#include <memory>
#include <cstdlib>
#include <cstring>
#include "Memory.hpp"

namespace ari
{
    namespace core
    {
		//------------------------------------------------------------------------------
		void*
			Memory::Alloc(int numBytes) {
			void* ptr = std::malloc(numBytes);
#if ARI_ALLOCATOR_DEBUG || ARI_UNITTESTS
			Memory::Fill(ptr, numBytes, ARI_MEMORY_DEBUG_BYTE);
#endif
			return ptr;
		}

		//------------------------------------------------------------------------------
		void
			Memory::Fill(void* ptr, int numBytes, uint8_t value) {
			std::memset(ptr, value, numBytes);
		}

		//------------------------------------------------------------------------------
		void*
			Memory::ReAlloc(void* ptr, int s) {
			return std::realloc(ptr, s);
		}

		//------------------------------------------------------------------------------
		void
			Memory::Free(void* p) {
			std::free(p);
		}

		//------------------------------------------------------------------------------
		void
			Memory::Copy(const void* from, void* to, int numBytes) {
			std::memcpy(to, from, numBytes);
		}

		//------------------------------------------------------------------------------
		void
			Memory::Move(const void* from, void* to, int numBytes) {
			std::memmove(to, from, numBytes);
		}

		//------------------------------------------------------------------------------
		void
			Memory::Clear(void* ptr, int numBytes) {
			std::memset(ptr, 0, numBytes);
		}
    } // namespace core
    
} // namespace ari
