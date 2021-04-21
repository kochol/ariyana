using System;
using System.Diagnostics;

namespace ari.core
{
	class FreeListAllocator: Allocator
	{
		public enum PlacementPolicy
		{
			FIND_FIRST,
			FIND_BEST
    	};

		struct FreeHeader
		{
		    public int blockSize;
		};

		struct AllocationHeader
		{
		    public int blockSize;
		    public int8 padding;
		};

		typealias Node = SinglyLinkedList<FreeHeader>.Node;

		void* m_start_ptr = null;
		PlacementPolicy m_pPolicy;
		SinglyLinkedList<FreeHeader> m_freeList = .();

		public this(uint32 totalSize, PlacementPolicy pPolicy): base(totalSize)
		{
			m_pPolicy = pPolicy;
		}

		public override void Init()
		{
			if (m_start_ptr != null)
			{
			    delete m_start_ptr;
			    m_start_ptr = null;
			}
			m_start_ptr = new uint8[m_totalSize]* (?);

			Reset();
		}

		public ~this()
		{
			delete m_start_ptr;
			m_start_ptr = null;
		}

		public override void* Allocate(int size, int alignment)
		{
		    int allocationHeaderSize = sizeof(AllocationHeader);
		    Debug.Assert(size >= sizeof(Node), "Allocation size must be bigger");

		    // Search through the free list for a free block that has enough space to allocate our data
		    int padding;
		    Node * affectedNode; 
		    Node * previousNode;
		    Find(size, alignment, out padding, out previousNode, out affectedNode);
		    if (affectedNode == null)
		        return null;


		    int alignmentPadding =  padding - allocationHeaderSize;
		    int requiredSize = size + padding;    

		    int rest = affectedNode.data.blockSize - requiredSize - alignmentPadding;

		    if (rest > allocationHeaderSize) 
		    {
		        // We have to split the block into the data block and a free block of size 'rest'
		        Node * newFreeNode = (Node*)(void*)((int)(void*)affectedNode + requiredSize + alignmentPadding);
		        newFreeNode.data.blockSize = rest;
		        m_freeList.Insert(affectedNode, newFreeNode);
		        rest = 0;
		    }
		    m_freeList.Remove(previousNode, affectedNode);

		    // Setup data block
		    int headerAddress = (int)(void*)affectedNode + alignmentPadding;
		    int dataAddress = headerAddress + allocationHeaderSize;
		    ((AllocationHeader *)(void*) headerAddress).blockSize = requiredSize;
		    ((AllocationHeader *)(void*) headerAddress).padding = (int8)(alignmentPadding + rest);

		    m_used += requiredSize;
		    m_peak = Math.Max(m_peak, m_used);

		    return (void*) dataAddress;
		}

		void Find(int size, int alignment, out int padding, out Node* previousNode, out Node* foundNode)
		{
		    switch (m_pPolicy) {
		        case .FIND_FIRST:
		            FindFirst(size, alignment, out padding, out previousNode, out foundNode);
		            break;
		        case .FIND_BEST:
		            FindBest(size, alignment, out padding, out previousNode, out foundNode);
		            break;
		    }
		}

		void FindFirst(int size, int alignment, out int padding, out Node* previousNode, out Node* foundNode)
		{
		    //Iterate list and return the first free block with a size >= than given size
		    Node* it = m_freeList.head;
			Node* itPrev = null;
			padding = 0;
		    
		    while (it != null)
			{
		        padding = Allocator.CalculatePaddingWithHeader((int)(void*)it, alignment, sizeof(AllocationHeader));
		        int requiredSpace = size + padding;
		        if (it.data.blockSize >= requiredSpace)
				{
		            break;
		        }
		        itPrev = it;
		        it = it.next;
		    }
		    previousNode = itPrev;
		    foundNode = it;
		}

		void FindBest(int size, int alignment, out int padding, out Node* previousNode, out Node* foundNode)
		{
		    // Iterate WHOLE list keeping a pointer to the best fit
		    int smallestDiff = int.MaxValue;
		    Node * bestBlock = null;
		    Node * it = m_freeList.head;
		    Node * itPrev = null;
			padding = 0;

		    while (it != null)
			{
		        padding = Allocator.CalculatePaddingWithHeader((int)(void*)it, alignment, sizeof (AllocationHeader));
		        int requiredSpace = size + padding;
		        if (it.data.blockSize >= requiredSpace && (it.data.blockSize - requiredSpace < smallestDiff))
				{
		            bestBlock = it;
		        }
		        itPrev = it;
		        it = it.next;
		    }
		    previousNode = itPrev;
		    foundNode = bestBlock;
		}


		public override void Free(void* ptr)
		{
			// Insert it in a sorted position by the address number
			int currentAddress = (int) ptr;
			int headerAddress = currentAddress - sizeof (AllocationHeader);
			AllocationHeader * allocationHeader = (AllocationHeader*)(void*)headerAddress;

			Node * freeNode = (Node *)(void*) (headerAddress);
			freeNode.data.blockSize = allocationHeader.blockSize + allocationHeader.padding;
			freeNode.next = null;

			Node * it = m_freeList.head;
			Node * itPrev = null;
			while (it != null)
			{
			    if (ptr < it)
				{
			        m_freeList.Insert(itPrev, freeNode);
			        break;
			    }
			    itPrev = it;
			    it = it.next;
			}

			m_used -= freeNode.data.blockSize;

			// Merge contiguous nodes
			Coalescence(itPrev, freeNode);  
		}

		void Coalescence(Node* previousNode, Node * freeNode)
		{   
		    if (freeNode.next != null && 
		            (int)(void*) freeNode + freeNode.data.blockSize == (int)(void*) freeNode.next)
			{
		        freeNode.data.blockSize += freeNode.next.data.blockSize;
		        m_freeList.Remove(freeNode, freeNode.next);
		    }
		    
		    if (previousNode != null &&
		            (int)(void*) previousNode + previousNode.data.blockSize == (int)(void*) freeNode)
			{
		        previousNode.data.blockSize += freeNode.data.blockSize;
		        m_freeList.Remove(previousNode, freeNode);
			}
		}

		public void Reset()
		{
			m_used = 0;
			m_peak = 0;
			Node * firstNode = (Node *) m_start_ptr;
			firstNode.data.blockSize = m_totalSize;
			firstNode.next = null;
			m_freeList.head = null;
			m_freeList.Insert(null, firstNode);
		}

#if BF_ENABLE_REALTIME_LEAK_CHECK
		protected override void GCMarkMembers()
		{
			GC.Mark(m_start_ptr, m_totalSize);
		}
#endif

	}
}
