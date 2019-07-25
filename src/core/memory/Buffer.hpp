#pragma once
#include <cstdint>

namespace ari::core
{
    class Buffer
    {
    public:
        /// default constructor
        Buffer();
        /// move constructor
        Buffer(Buffer&& rhs);
        /// destructor
        ~Buffer();

        /// always force move-construct
        Buffer(const Buffer& rhs) = delete;
        /// always force move-assign
        void operator=(const Buffer& rhs) = delete;

        /// move-assignment
        void operator=(Buffer&& rhs);

        /// get number of bytes in buffer
        int Size() const;
        /// return true if empty
        bool Empty() const;
        /// get capacity in bytes of buffer
        int Capacity() const;
        /// get number of free bytes at back
        int Spare() const;

        /// make room for N more bytes
        void Reserve(int numBytes);
        /// add bytes to buffer
        void Add(const uint8_t* data, int numBytes);
        /// add uninitialized bytes to buffer, return pointer to start
        uint8_t* Add(int numBytes);
        /// remove a chunk of data from the buffer, return number of bytes removed
        int Remove(int offset, int numBytes);
        /// clear the buffer (deletes content, keeps capacity)
        void Clear();
        /// get read-only pointer to content (throws assert if would return nullptr)
        const uint8_t* Data() const;
        /// get read/write pointer to content (throws assert if would return nullptr)
        uint8_t* Data();

		/// Read size bytes from buffer and copy to data.
		int Read(void* data, int _size);
		/// Seek the buffer to the desired position
		bool Seek(int offset);
		/// Tell the where is the buffer offset
		int Tell() const;

    private:
        /// (re-)allocate buffer
        void alloc(int newCapacity);
        /// destroy buffer
        void destroy();
        /// append-copy content into currently allocated buffer, bump size
        void copy(const uint8_t* ptr, int numBytes);

        int size;
        int capacity;
        uint8_t* data;
		int pos;

    };
    
} // namespace ari::core
