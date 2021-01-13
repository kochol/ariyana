using System;

namespace ari
{
	public static class Hash
	{
		[CLink]
		public static extern uint32 HashStringFNV32(char8* _str);
	}
}
