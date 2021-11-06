using System.Collections;
using System;

namespace ari.core
{
	class MemoryPool
	{
		List<FreeListAllocator> allocators = new List<FreeListAllocator>() ~ Shutdown();
		Dictionary<int, int> objects = new Dictionary<int, int>() ~ delete _;

		public int MinGrow = 65536; // 64Kb
		public int MaxGrow = 1048576; // 1MB
		int LastGrow = MinGrow;

		public void Setup(int size)
		{
			if (allocators.Count > 0)
				return;

			var a = new FreeListAllocator((uint32)size, .FIND_FIRST);
			a.Init();
			allocators.Add(a);
			LastGrow = size;
		}

		public void Shutdown()
		{
			DeleteContainerAndItems!(allocators);
		}

		public void* Alloc(int size, int align)
		{
			for (int i = 0; i < allocators.Count; i++)
			{
				let r = allocators[i].Allocate(size, align);
				if (r != null)
				{
					objects.Add((int)r, i);
					return r;
				}
			}

			LastGrow *= 2;
			if (LastGrow < MinGrow)
				LastGrow = MinGrow;
			else if (LastGrow > MaxGrow)
				LastGrow = MaxGrow;

			var a = new FreeListAllocator((uint32)LastGrow, .FIND_FIRST);
			a.Init();
			allocators.Add(a);
			let r = a.Allocate(size, align);
			objects.Add((int)r, allocators.Count - 1);
			return r;
		}

		public void Free(void* ptr)
		{
			let i = (int)ptr;
			let a = objects.GetAndRemove(i).Value.value;
			allocators[a].Free(ptr);
		}
	}

	class MemoryPool<T>
	{
		static MemoryPool pool = new MemoryPool() ~ delete _;

		public static MemoryPool Pool {
			get => pool;
		}
	}
}
