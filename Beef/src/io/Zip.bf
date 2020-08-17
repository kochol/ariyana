using System;

namespace ari.io
{
	class Zip
	{
		[CLink]
		static extern uint8* Zip_Compress(uint8* data, ref int32 in_out_size);

		[CLink]
		static extern uint8* Zip_Decompress(uint8* data, ref int32 in_out_size);

		public static uint8* Compress(uint8* data, ref int32 in_out_size)
		{
			return Zip_Compress(data, ref in_out_size);
		}

		public static uint8* Decompress(uint8* data, ref int32 in_out_size)
		{
			return Zip_Decompress(data, ref in_out_size);
		}
	}
}
