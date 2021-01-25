using System;

namespace ari
{
	public static class Hash
	{
		const uint32 FNV1_32_INIT = 0x811c9dc5;
		const uint32 FNV1_32_PRIME = 0x01000193;

		public static uint32 HashStringFNV32(char8* _str)
		{
			char8* s = _str;

			uint32 hval = FNV1_32_INIT;
			while (*s > 0) {
			    hval ^= (uint32)*s++;
			    hval *= FNV1_32_PRIME;
			}

			return hval;
		}
	} 
}
