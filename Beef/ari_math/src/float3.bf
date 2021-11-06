using System;

namespace ari.math
{
	struct float3
	{
		public float x, y, z;

		[Inline]
		public this()
		{
			x = y = z = 0;
		}

		[Inline]
		public this(float x, float y, float z)
		{
			this.x = x;
			this.y = y;
			this.z = z;
		}

		[Inline]
		public static float3 operator - (in float3 a, in float3 b)
		{
			return .(a.x - b.x, a.y - b.y, a.z - b.z);
		}

		[Inline]
		public static float operator * (in float3 a, in float3 b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z;
		}

		[Inline]
		public float3 GetCross(in float3 b)
		{
			return .(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
		}

		[Inline]
		public float Length()
		{
			return Math.Sqrt(this * this);
		}

		[Inline]
		public float3 GetNormal()
		{
			let len = Length();
			if (len > 0.0f) {
			    let invlen = 1.0f / len;
			    return .(x * invlen, y * invlen, z * invlen);
			} else {
				Runtime.Assert(false, "Divide by zero");
			    return .(0.0f, 0.0f, 0.0f);
			}
		}
	}
}
