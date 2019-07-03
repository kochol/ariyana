#pragma once
#include "FreeListAllocator.h"
#include "Memory.hpp"
#include "../containers/Array.hpp"

namespace ari::core
{
    template<class T>
    class MemoryPool
    {
    private:

		static bool						m_bValid;
		static FreeListAllocator	*	m_pAlloc;
		static Array<T*>				m_vObjects;

    public:

		/// Is the pool initialized and valid?
		static bool IsValid()
		{
			return m_bValid;
		}

		/**
		 * \brief 
		 * \param size 
		 */
		static void Setup(size_t size)
		{
			if (m_bValid)
				return;
			m_pAlloc = Memory::New<FreeListAllocator>(size, FreeListAllocator::FIND_FIRST);
			m_pAlloc->Init();
			m_bValid = true;
		}

		static void Shutdown()
		{
			if (m_pAlloc)
			{
				Memory::Delete(m_pAlloc);
				m_pAlloc = nullptr;
			}
			m_bValid = false;
			m_vObjects.Clear();
		}

		template<typename TYPE, typename... ARGS>
		static T* New(const uint32_t& index, ARGS&& ... args) 
    	{
			T* ptr = (T*)m_pAlloc->Allocate(sizeof(TYPE), 8);
			
			if (index < (uint32_t)m_vObjects.Size())
				m_vObjects[index] = ptr;
			else
				m_vObjects.Add(ptr);

			return new(ptr) TYPE(std::forward<ARGS>(args)...);
		};

		/// replacement delete (see Memory::New())
		static void Delete(T* ptr) {
			ptr->~TYPE();
			m_pAlloc->Free(ptr);
		};

		static T* GetByIndex(const uint32_t& index)
		{
			return m_vObjects[index];
		}
    };

	template<class T>
	bool MemoryPool<T>::m_bValid = false;

	template<class T>
	FreeListAllocator* MemoryPool<T>::m_pAlloc = nullptr;

	template<class T>
	Array<T*> MemoryPool<T>::m_vObjects;

} // namespace ari::core
