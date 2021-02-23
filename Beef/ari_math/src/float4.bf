using System;
using System.Numerics;

namespace ari.math
{
	[Union]
	struct float4
	{
		public v128 vec;
		public float[4] v;

		[Inline]
		public this()
		{
			this.v = .(0.0f, 0.0f, 0.0f, 1.0f);
		}

		[Inline]
		public this(float x, float y, float z, float w)
		{
			this.v = .(x, y, z, w);
		}
	}
}
