using System;

namespace ari.core
{
	class Memory
	{
		[LinkName("ARI_FREE_MEM")]
		public static extern void Free(void* ptr);
	}
}
