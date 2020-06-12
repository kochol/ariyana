using System;

namespace ari
{
	[CRepr]
	public struct Vector2
	{
		public float x;
		public float y;

		public this(float _x, float _y)
		{
			x = _x;
			y = _y;
		}

		public void Set(float _x, float _y) mut
		{
			x = _x;
			y = _y;
		}

		public void Set(float _v) mut
		{
			x = y = _v;
		}

		public static Vector2 operator * (Vector2 lhd, float rhd)
		{
			return Vector2(lhd.x * rhd, lhd.y * rhd);
		}

		public static Vector2 operator + (Vector2 lhd, float rhd)
		{
			return Vector2(lhd.x + rhd, lhd.y + rhd);
		}

		public static Vector2 operator + (Vector2 lhd, Vector2 rhd)
		{
			return Vector2(lhd.x + rhd.x, lhd.y + rhd.y);
		}
	}

	[CRepr]
	public struct Vector3
	{
		public float x;
		public float y;
		public float z;

		public void Set(float _x, float _y, float _z) mut
		{
			x = _x;
			y = _y;
			z = _z;
		}

		public void Set(float _v) mut
		{
			x = y = z = _v;
		}
	}

	[CRepr]
	public struct Vector4
	{
		public float x;
		public float y;
		public float z;
		public float w;

		public void Set(float _x, float _y, float _z, float _w) mut
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}

		public void Set(float _v) mut
		{
			x = y = z = w = _v;
		}
	}

	[CRepr]
	public struct Quat
	{
		public float x;
		public float y;
		public float z;
		public float w;

		public this(float _x, float _y, float _z, float _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}

	}

	[CRepr]
	public struct Color
	{
		public float r;
		public float g;
		public float b;
		public float a;

		public this(float _r, float _g, float _b, float _a)
		{
			r = _r;
			g = _g;
			b = _b;
			a = _a;
		}

		public this(int32 _r, int32 _g, int32 _b, int32 _a)
		{
			r = (float)_r / 255.0f;
			g = (float)_g / 255.0f;
			b = (float)_b / 255.0f;
			a = (float)_a / 255.0f;
		}

		public static readonly Color LIGHTGRAY = Color(0.784f, 0.784f, 0.784f, 1.0f);
		public static readonly Color GRAY = Color(0.509f, 0.509f, 0.509f, 1.0f);
		public static readonly Color DARKGRAY = Color(0.313f, 0.313f, 0.313f, 1.0f);
		public static readonly Color YELLOW = Color(0.992f, 0.976f, 0.0f, 1.0f);
		public static readonly Color GOLD = Color(1.0f, 0.796f, 0.0f, 1.0f);
		public static readonly Color ORANGE = Color(1.0f, 0.631f, 0.0f, 1.0f);
		public static readonly Color PINK = Color(1.0f, 0.427f, 0.76f, 1.0f);
		public static readonly Color RED = Color(0.901f, 0.16f, 0.215f, 1.0f);
		public static readonly Color MAROON = Color(0.745f, 0.129f, 0.215f, 1.0f);
		public static readonly Color GREEN = Color(0.0f, 0.894f, 0.188f, 1.0f);
		public static readonly Color LIME = Color(0.0f, 0.619f, 0.184f, 1.0f);
		public static readonly Color DARKGREEN = Color(0.0f, 0.458f, 0.172f, 1.0f);
		public static readonly Color SKYBLUE = Color(0.4f, 0.749f, 1.0f, 1.0f);
		public static readonly Color BLUE = Color(0.0f, 0.474f, 0.945f, 1.0f);
		public static readonly Color DARKBLUE = Color(0.0f, 0.321f, 0.674f, 1.0f);
		public static readonly Color PURPLE = Color(0.784f, 0.748f, 1.0f, 1.0f);
		public static readonly Color VIOLET = Color(0.529f, 0.235f, 0.745f, 1.0f);
		public static readonly Color DARKPURPLE = Color(0.439f, 0.121f, 0.494f, 1.0f);
		public static readonly Color BEIGE = Color(0.827f, 0.69f, 0.513f, 1.0f);
		public static readonly Color BROWN = Color(0.498f, 0.415f, 0.309f, 1.0f);
		public static readonly Color DARKBROWN = Color(0.298f, 0.247f, 0.184f, 1.0f);
		public static readonly Color WHITE = Color(1.0f, 1.0f, 1.0f, 1.0f);
		public static readonly Color BLACK = Color(0.0f, 0.0f, 0.0f, 1.0f);
		public static readonly Color BLANK = Color(0.0f, 0.0f, 0.0f, 0.0f);
		public static readonly Color MAGENTA = Color(1, 0.0f, 1, 1.0f);
		public static readonly Color RAYWHITE = Color(0.9607f, 0.9607f, 0.9607f, 1.0f);
	}

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
