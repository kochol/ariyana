using System;

namespace ari.core
{
	class FreeListAllocator
	{
		public enum PlacementPolicy {
			FIND_FIRST,
			FIND_BEST
    	};

		void* obj = null;

		[CLink]
		static extern void* CreateFreeListAllocator(uint32 totalSize, uint32 pPolicy); // 0: First, 1: Best
		[CLink]
		static extern void DeleteFreeListAllocator(void* obj);
		[CLink]
		static extern void* FreeListAllocate(void* obj, uint32 size, uint32 alignment);
		[CLink]
		static extern void FreeListFree(void* obj, void* ptr);
		[CLink]
		static extern void FreeListInit(void* obj);
		[CLink]
		static extern void FreeListReset(void* obj);

		public this(uint32 totalSize, PlacementPolicy pPolicy)
		{
			obj = CreateFreeListAllocator(totalSize, (uint32)pPolicy);
		}

		public ~this()
		{
			DeleteFreeListAllocator(obj);
		}

		public void* Alloc(int size, int align)
		{
			return FreeListAllocate(obj, (uint32)size, (uint32)align);
		}

		public void Free(void* ptr)
		{
			FreeListFree(obj, ptr);
		}

		public void Init()
		{
			FreeListInit(obj);
		}

		public void Reset()
		{
			FreeListReset(obj);
		}
	}
}
