using System;

namespace ari.math
{
	[Union]
	struct float4x4
	{
		public float4[4] col;
		public float[16] f;
		public M m;
		public struct M
		{
			public float m11, m21, m31, m41;
			public float m12, m22, m32, m42;
			public float m13, m23, m33, m43;
			public float m14, m24, m34, m44;

			[Inline]
			public this()
			{
				this.m11 = this.m22 = this.m33 = this.m44 = 1.0f;
				this.m12 = this.m13 = this.m14 = 0.0f;
				this.m21 = this.m23 = this.m24 = 0.0f;
				this.m31 = this.m32 = this.m34 = 0.0f;
				this.m41 = this.m42 = this.m43 = 0.0f;
			}

			[Inline]
			public this(float m11, float m12, float m13, float m14,
						float m21, float m22, float m23, float m24,
						float m31, float m32, float m33, float m34,
			        	float m41, float m42, float m43, float m44)
			{
				this.m11 = m11;
				this.m12 = m12;
				this.m13 = m13;
				this.m14 = m14;
				this.m21 = m21;
				this.m22 = m22;
				this.m23 = m23;
				this.m24 = m24;
				this.m31 = m31;
				this.m32 = m32;
				this.m33 = m33;
				this.m34 = m34;
				this.m41 = m41;
				this.m42 = m42;
				this.m43 = m43;
				this.m44 = m44;
			}
		}

		[Inline]
		public this()
		{
			this.m = .();
		}

		[Inline]
		public this(float4 v1, float4 v2, float4 v3, float4 v4)
		{
			this.col = .(v1, v2, v3, v4);
		}

		[Inline]
		public this(float m11, float m12, float m13, float m14,
					float m21, float m22, float m23, float m24,
					float m31, float m32, float m33, float m34,
                	float m41, float m42, float m43, float m44)
		{
			this.m = .(
				m11, m12, m13, m14,
				m21, m22, m23, m24,
				m31, m32, m33, m34,
				m41, m42, m43, m44
				);
		}
		/// Set the scale rotation and translate
		[Inline]
		public void SRT(float _sx, float _sy, float _sz,
						float _ax, float _ay, float _az,
						float _tx, float _ty, float _tz) mut
		{
			float sx, cx, sy, cy, sz, cz;

			if (_ax != 0) {
			    sx = Math.Sin(_ax);
			    cx = Math.Cos(_ax);
			} else {
			    sx = 0;
			    cx = 1.0f;
			}

			if (_ay != 0) {
			    sy = Math.Sin(_ay);
			    cy = Math.Cos(_ay);
			} else {
			    sy = 0;
			    cy = 1.0f;
			}

			if (_az != 0) {
			    sz = Math.Sin(_az);
			    cz = Math.Cos(_az);
			} else {
			    sz = 0;
			    cz = 1.0f;
			}

			float sxsz = sx * sz;
			float cycz = cy * cz;

			m.m11 = _sx * (cycz - sxsz * sy);
			m.m12 = _sx * -cx * sz;
			m.m13 = _sx * (cz * sy + cy * sxsz);
			m.m14 = _tx;
			m.m21 = _sy * (cz * sx * sy + cy * sz);
			m.m22 = _sy * cx * cz;
			m.m23 = _sy * (sy * sz - cycz * sx);
			m.m24 = _ty;
			m.m31 = _sz * -cx * sy;
			m.m32 = _sz * sx;
			m.m33 = _sz * cx * cy;
			m.m34 = _tz;
			m.m41 = m.m42 = m.m43 = 0.0f;
			m.m44 = 1.0f;
		}

		[Inline]
		public static float4x4 operator * (in float4x4 a, in float4x4 b)
		{
			return .(a * b.col[0], a * b.col[1], a * b.col[2], a * b.col[3]);
		}

		[Inline]
		public static float4 operator * (in float4x4 m, in float4 v)
		{
			return .(
				v.v[0] * m.m.m11 + v.v[1] * m.m.m12 + v.v[2] * m.m.m13 + v.v[3] * m.m.m14,
				v.v[0] * m.m.m21 + v.v[1] * m.m.m22 + v.v[2] * m.m.m23 + v.v[3] * m.m.m24,
				v.v[0] * m.m.m31 + v.v[1] * m.m.m32 + v.v[2] * m.m.m33 + v.v[3] * m.m.m34,
				v.v[0] * m.m.m41 + v.v[1] * m.m.m42 + v.v[2] * m.m.m43 + v.v[3] * m.m.m44
				);
		}

		public static float4x4 Ortho(float width, float height, float zn, float zf, float offset, bool ogl_ndc)
		{
		    let d = zf - zn;
		    let cc = (ogl_ndc ? 2.0f : 1.0f) / d;
		    let ff = ogl_ndc ? -(zn + zf) / d : -zn / d;
		
		    return .(2.0f / width, 0, 0, offset, 0, 2.0f / height, 0, 0, 0, 0, -cc, ff, 0, 0, 0, 1.0f);
		}

		public static float4x4 ViewLookat(in float3 eye, in float3 target, in float3 up)
		{
			float3 zaxis = (target - eye).GetNormal();
			float3 xaxis = zaxis.GetCross(up).GetNormal();
			float3 yaxis = xaxis.GetCross(zaxis);

			return .(xaxis.x,    xaxis.y,    xaxis.z,    -(xaxis * eye), 
			         yaxis.x,    yaxis.y,    yaxis.z,    -(yaxis * eye), 
			         -zaxis.x,   -zaxis.y,   -zaxis.z,    zaxis * eye,
			         0,          0,          0,           1.0f);
		}
	}
}
