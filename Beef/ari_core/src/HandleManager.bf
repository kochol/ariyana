using System.Collections;
using System;

namespace ari.core
{
	public class HandleManager<T>
	{
		static Dictionary<uint32, T> handles = new Dictionary<uint32, T>() ~ delete _;
		static uint32 LastHandle = 0;

		public static uint32 CreateHandle(T obj)
		{
			handles.Add(LastHandle, obj);
			return LastHandle++;
		}

		public static T GetByHandle(ref uint32 handle)
		{
			return handles[handle];
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
