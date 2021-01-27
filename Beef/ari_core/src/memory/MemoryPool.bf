using System.Collections;

namespace ari.core
{
	class MemoryPool<T>
	{
		static List<FreeListAllocator> allocators = new List<FreeListAllocator>() ~ Shutdown();
		static Dictionary<int, int> objects = new Dictionary<int, int>() ~ delete _;

		public static int MinGrow = 65536; // 64Kb
		public static int MaxGrow = 1048576; // 1MB
		static int LastGrow = MinGrow;

		public static void Setup(int size)
		{
			if (allocators.Count > 0)
				return;

			var a = new FreeListAllocator((uint32)size, .FIND_FIRST);
			a.Init();
			allocators.Add(a);
			LastGrow = size;
		}

		public static void Shutdown()
		{
			DeleteContainerAndItems!(allocators);
		}

		public static void* Alloc(int size, int align)
		{
			for (int i = 0; i < allocators.Count; i++)
			{
				let r = allocators[i].Alloc(size, align);
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
			let r = a.Alloc(size, align);
			objects.Add((int)r, allocators.Count - 1);
			return r;
		}

		public static void Free(void* ptr)
		{
			let i = (int)ptr;
			let a = objects.GetAndRemove(i).Value.value;
			allocators[a].Free(ptr);
		}
	}
}
