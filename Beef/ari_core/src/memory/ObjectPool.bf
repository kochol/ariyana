using System;
using System.Collections;

namespace ari.core
{
	class ObjectPool
	{
		List<PoolAllocator> allocators = new List<PoolAllocator>() ~ Shutdown();
		Dictionary<int, int> objects = new Dictionary<int, int>() ~ delete _;

		public int MinGrow = 100;
		public int MaxGrow = 1000;
		int LastGrow = MinGrow;
		int data_size;

		protected this(int _data_size)
		{
			data_size = _data_size;
		}

		public void Setup(int count)
		{
			if (allocators.Count > 0)
				return;

			var a = new PoolAllocator((uint32)(count * data_size), (uint32)data_size);
			a.Init();
			allocators.Add(a);
			LastGrow = count;
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

			var a = new PoolAllocator((uint32)(LastGrow * data_size), (uint32)data_size);
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

	class ObjectPool<T>
	{
		static ObjectPool pool = new [Friend]ObjectPool(typeof(T).InstanceSize) ~ delete _;

		public static ObjectPool Pool {
			get => pool;
		}
	}
}
