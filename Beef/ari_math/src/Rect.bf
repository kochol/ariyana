using System;

namespace ari.math
{
	[CRepr]
	public struct Rect<T>
	{
		public T x;
		public T y;
		public T width;
		public T height;
	}

	typealias RectI = Rect<int32>;
}
