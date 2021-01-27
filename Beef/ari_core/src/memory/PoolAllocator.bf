using System;

namespace ari.core
{
	class PoolAllocator
	{
		void* obj = null;
		bool valid = false;

		public bool Valid
		{
			get => valid;
		}

		[CLink]
		static extern void* CreatePoolAllocator(uint32 totalSize, uint32 chunkSize);
		[CLink]
		static extern void DeletePoolAllocator(void* obj);
		[CLink]
		static extern void* PoolAllocate(void* obj, uint32 size, uint32 alignment);
		[CLink]
		static extern void PoolFree(void* obj, void* ptr);
		[CLink]
		static extern void PoolInit(void* obj);
		[CLink]
		static extern void PoolReset(void* obj);

		public this(uint32 totalSize, uint32 chunkSize)
		{
			obj = CreatePoolAllocator(totalSize, chunkSize);
		}

		public ~this()
		{
			DeletePoolAllocator(obj);
		}

		public void* Alloc(int size, int align)
		{
			return PoolAllocate(obj, (uint32)size, (uint32)align);
		}

		public void Free(void* ptr)
		{
			PoolFree(obj, ptr);
		}

		public void Init()
		{
			PoolInit(obj);
			valid = true;
		}

		public void Reset()
		{
			PoolReset(obj);
		}
	}
}
