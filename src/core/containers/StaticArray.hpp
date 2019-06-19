#pragma once
#include "../Assertion.hpp"

namespace ari
{
	namespace core
	{
		template <typename TYPE, int SIZE>
		struct StaticArray
		{
			/// default constructor
			StaticArray() = default;
			/// copy constructor
			StaticArray(const StaticArray& rhs);
			/// move constructor
			StaticArray(StaticArray&& rhs);
			/// Destructor
			~StaticArray() = default;

			/// copy-assignment operator
			void operator=(const StaticArray& rhs);
			/// move-assignment operator
			void operator=(StaticArray&& rhs);
			
			/// get number of elements in array
			int Capacity() const;
			/// get count of used elements in array
			int Count() const;

			/// Add a new element to array and returns it reference.
			TYPE& Add(int& index);

			/// read/write access single element
			TYPE& operator[](int index);
			/// read-only access single element
			const TYPE& operator[](int index) const;

			/// fill the array with a value
			void Fill(const TYPE& val);

			/// C++ conform begin
			TYPE* begin();
			/// C++ conform begin
			const TYPE* begin() const;
			/// C++ conform end
			TYPE* end();
			/// C++ conform end
			const TYPE* end() const;

		private:
			TYPE	m_Items[SIZE];
			int		m_iCount = 0;
		};

		//------------------------------------------------------------------------------
		template<class TYPE, int SIZE>
		StaticArray<TYPE, SIZE>::StaticArray(const StaticArray& rhs) {
			for (int i = 0; i < SIZE; i++) {
				this->m_Items[i] = rhs.m_Items[i];
			}
		}

		//------------------------------------------------------------------------------
		template<class TYPE, int SIZE>
		StaticArray<TYPE, SIZE>::StaticArray(StaticArray&& rhs) {
			for (int i = 0; i < SIZE; i++) {
				this->m_Items[i] = std::move(rhs.m_Items[i]);
			}
		}

		//------------------------------------------------------------------------------
		template<class TYPE, int SIZE> void
			StaticArray<TYPE, SIZE>::operator=(const StaticArray& rhs) {
			for (int i = 0; i < SIZE; i++) {
				this->m_Items[i] = rhs.m_Items[i];
			}
		}

		//------------------------------------------------------------------------------
		template<class TYPE, int SIZE> void
			StaticArray<TYPE, SIZE>::operator=(StaticArray&& rhs) {
			for (int i = 0; i < SIZE; i++) {
				this->m_Items[i] = std::move(rhs.m_Items[i]);
			}
		}

		//------------------------------------------------------------------------------
		template<class TYPE, int SIZE> int
			StaticArray<TYPE, SIZE>::Capacity() const 
		{
			return SIZE;
		}

		//------------------------------------------------------------------------------
		template <typename TYPE, int SIZE>
		int StaticArray<TYPE, SIZE>::Count() const
		{
			return m_iCount;
		}

		//------------------------------------------------------------------------------
		template <typename TYPE, int SIZE>
		TYPE& StaticArray<TYPE, SIZE>::Add(int& index)
		{
			a_assert(m_iCount + 1 < SIZE);
			return m_Items[m_iCount++];
		}

		//------------------------------------------------------------------------------
		template<class TYPE, int SIZE> TYPE&
			StaticArray<TYPE, SIZE>::operator[](int index) {
			a_assert_range_dbg(index, SIZE);
			return this->m_Items[index];
		}

		//------------------------------------------------------------------------------
		template<class TYPE, int SIZE> const TYPE&
			StaticArray<TYPE, SIZE>::operator[](int index) const {
			a_assert_range_dbg(index, SIZE);
			return this->m_Items[index];
		}

		//------------------------------------------------------------------------------
		template<class TYPE, int SIZE> void
			StaticArray<TYPE, SIZE>::Fill(const TYPE& val) {
			for (int i = 0; i < SIZE; i++) {
				this->m_Items[i] = val;
			}
		}

		//------------------------------------------------------------------------------
		template<class TYPE, int SIZE> TYPE*
			StaticArray<TYPE, SIZE>::begin() {
			return &(this->m_Items[0]);
		}

		//------------------------------------------------------------------------------
		template<class TYPE, int SIZE> const TYPE*
			StaticArray<TYPE, SIZE>::begin() const {
			return &(this->m_Items[0]);
		}

		//------------------------------------------------------------------------------
		template<class TYPE, int SIZE> TYPE*
			StaticArray<TYPE, SIZE>::end() {
			return &(this->m_Items[SIZE]);
		}

		//------------------------------------------------------------------------------
		template<class TYPE, int SIZE> const TYPE*
			StaticArray<TYPE, SIZE>::end() const {
			return &(this->m_Items[SIZE]);
		}
		
	} // namespace core
	
} // namespace ari
