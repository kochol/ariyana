#include "Buffer.hpp"
#include "Memory.hpp"
#include "core/Assertion.hpp"

namespace ari::core
{
	//------------------------------------------------------------------------------
	Buffer::Buffer(): size(0), capacity(0), data(nullptr), pos(0)
	{
	}

	//------------------------------------------------------------------------------
	Buffer::Buffer(Buffer&& rhs): size(rhs.size), capacity(rhs.capacity), data(rhs.data)
		, pos(rhs.pos)
	{
		rhs.size = 0;
		rhs.capacity = 0;
		rhs.data = nullptr;
		rhs.pos = 0;
	}

	//------------------------------------------------------------------------------
	Buffer::~Buffer()
	{
		destroy();
	}

	//------------------------------------------------------------------------------
	void Buffer::operator=(Buffer&& rhs)
	{
		this->destroy();
		this->size = rhs.size;
		this->capacity = rhs.capacity;
		this->data = rhs.data;
		this->pos = rhs.pos;
		rhs.size = 0;
		rhs.capacity = 0;
		rhs.data = nullptr;
		rhs.pos = 0;
	}

	//------------------------------------------------------------------------------
	int Buffer::Size() const
	{
		return size;
	}

	//------------------------------------------------------------------------------
	bool Buffer::Empty() const
	{
		return size == 0;
	}

	//------------------------------------------------------------------------------
	int Buffer::Capacity() const
	{
		return capacity;
	}

	//------------------------------------------------------------------------------
	int Buffer::Spare() const
	{
		return capacity - size;
	}

	//------------------------------------------------------------------------------
	void Buffer::Reserve(int numBytes)
	{
		// need to grow?
		if ((this->size + numBytes) > this->capacity) {
			const int newCapacity = this->size + numBytes;
			this->alloc(newCapacity);
		}
	}

	//------------------------------------------------------------------------------
	void Buffer::Add(const uint8_t* data, int numBytes)
	{
		this->Reserve(numBytes);
		this->copy(data, numBytes);
	}

	//------------------------------------------------------------------------------
	uint8_t* Buffer::Add(int numBytes)
	{
		this->Reserve(numBytes);
		uint8_t* ptr = this->data + this->size;
		this->size += numBytes;
		return ptr;
	}

	//------------------------------------------------------------------------------
	int Buffer::Remove(int offset, int numBytes)
	{
		a_assert_dbg(offset >= 0);
		a_assert_dbg(numBytes >= 0);
		if (offset >= this->size) {
			return 0;
		}
		if ((offset + numBytes) >= this->size) {
			numBytes = this->size - offset;
		}
		a_assert_dbg((offset + numBytes) <= this->size);
		a_assert_dbg(numBytes >= 0);
		if (numBytes > 0) {
			int bytesToMove = this->size - (offset + numBytes);
			if (bytesToMove > 0) {
				Memory::Move(this->data + offset + numBytes, this->data + offset, bytesToMove);
			}
			this->size -= numBytes;
			a_assert_dbg(this->size >= 0);
		}
		return numBytes;
	}

	//------------------------------------------------------------------------------
	void Buffer::Clear()
	{
		size = 0;
		pos = 0;
	}

	//------------------------------------------------------------------------------
	const uint8_t* Buffer::Data() const
	{
		a_assert(data);
		return data;
	}

	//------------------------------------------------------------------------------
	uint8_t* Buffer::Data()
	{
		a_assert(data);
		return data;
	}

	//------------------------------------------------------------------------------
	int Buffer::Read(void* data, int _size)
	{
		if (_size > size - pos)
			_size = size - pos;

		core::Memory::Copy(reinterpret_cast<void*>(Data() + pos), data, _size);
		pos += _size;

		return _size;
	}

	//------------------------------------------------------------------------------
	bool Buffer::Seek(int offset)
	{
		if (offset > size || offset < 0)
			return false;

		pos = offset;
		return true;
	}

	int Buffer::Tell() const
	{
		return pos;
	}

	//------------------------------------------------------------------------------
	void Buffer::alloc(int newCapacity)
	{
		a_assert_dbg(newCapacity > this->capacity);
		a_assert_dbg(newCapacity > this->size);

		uint8_t* newBuf = (uint8_t*)Memory::Alloc(newCapacity);
		if (this->size > 0) {
			a_assert_dbg(this->data);
			Memory::Copy(this->data, newBuf, this->size);
		}
		if (this->data) {
			Memory::Free(this->data);
		}
		this->data = newBuf;
		this->capacity = newCapacity;
	}

	//------------------------------------------------------------------------------
	void Buffer::destroy()
	{
		if (this->data) {
			Memory::Free(this->data);
		}
		this->data = nullptr;
		this->size = 0;
		this->capacity = 0;
		this->pos = 0;
	}

	//------------------------------------------------------------------------------
	void Buffer::copy(const uint8_t* ptr, int numBytes)
	{
		// NOTE: it is valid to call copy with numBytes==0
		a_assert_dbg(this->data);
		a_assert_dbg((this->size + numBytes) <= this->capacity);
		Memory::Copy(ptr, this->data + this->size, numBytes);
		this->size += numBytes;
	}


} // namespace ari::core
