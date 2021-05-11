using System;
using System.Diagnostics;

namespace ari.core
{
	class PoolAllocator: Allocator
	{
		bool valid = false;
		struct  FreeHeader {};
		typealias Node = StackLinkedList<FreeHeader>.Node;
		StackLinkedList<FreeHeader> m_freeList;

		void * m_start_ptr = null;
		int m_chunkSize;

		public bool Valid
		{
			get => valid;
		}

		public this(uint32 totalSize, uint32 chunkSize): base(totalSize)
		{
			Debug.Assert(chunkSize >= 8, "Chunk size must be greater or equal to 8");
			Debug.Assert(totalSize % chunkSize == 0, "Total Size must be a multiple of Chunk Size");
			this.m_chunkSize = chunkSize;
		}

		public ~this()
		{
			delete m_start_ptr;
		}

		public override void* Allocate(int size, int align)
		{
			Debug.Assert(size == this.m_chunkSize, "Allocation size must be equal to chunk size");

		    Node * freePosition = m_freeList.Pop();

		    if (freePosition == null) // The pool allocator is full
		        return null;

		    m_used += m_chunkSize;
		    m_peak = Math.Max(m_peak, m_used);

			return (void*) freePosition;
		}

		public override void Free(void* ptr)
		{
			m_used -= m_chunkSize;

			m_freeList.Push((Node *) ptr);
		}

		public override void Init()
		{
			m_start_ptr = new uint8[m_totalSize]* (?);
			this.Reset();
			valid = true;
		}

		public void Reset()
		{
			m_used = 0;
			m_peak = 0;
			// Create a linked-list with all free positions
			int nChunks = m_totalSize / m_chunkSize;
			for (int i = 0; i < nChunks; ++i)
			{
			    int address = (int) m_start_ptr + i * m_chunkSize;
			    m_freeList.Push((Node *)(void*) address);
			}
		}

#if BF_ENABLE_REALTIME_LEAK_CHECK
		protected override void GCMarkMembers()
		{
			GC.Mark(m_start_ptr, m_totalSize);
		}
#endif

	}
}
