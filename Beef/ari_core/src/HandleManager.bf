using System.Collections;

namespace ari.core
{
	class HandleManager<T>
	{
		static Dictionary<uint32, void*> handles = new Dictionary<uint32, void*>() ~ delete _;
		static uint32 LastHandle = 0;

		public static uint32 CreateHandle(void* ptr)
		{
			handles.Add(LastHandle, ptr);
			return LastHandle++;
		}

		public static bool IsValid(ref uint32 handle)
		{
			let r = handles.ContainsKey(handle);
			if (!r)
				handle = uint32.MaxValue;
			return r;
		}

		public static void RemoveHandle(ref uint32 handle)
		{
			handles.Remove(handle);
		}
	}
}
