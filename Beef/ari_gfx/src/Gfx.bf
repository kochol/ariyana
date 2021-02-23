using System;
using ari.io;
using ari.math;

namespace ari
{
	[CRepr]
	public struct GfxSetup
	{
		public WindowSetup window = .();
		public int32 sample_count = 0;                   /* MSAA sample count */
		public int32 swap_interval = 0;                  /* the preferred swap interval (ignored on some platforms) */
	}

	[CRepr]
	public struct TextureHandle
	{
		public uint32 Handle = uint32.MaxValue;
		public uint32 Index = uint32.MaxValue;

		[CLink]
		static extern bool IsValidTexture(ref uint32 _entityHandle);

		public bool IsValid() mut
		{
			if (Handle == uint32.MaxValue || Index == uint32.MaxValue)
				return false;

			return IsValidTexture(ref Handle);
		}
	}

	[CRepr]
	public struct SubMeshHandle
	{
		public uint32 Handle = uint32.MaxValue;
		public uint32 Index = uint32.MaxValue;

		[CLink]
		static extern bool IsValidSubMesh(ref uint32 _entityHandle);

		public bool IsValid() mut
		{
			if (Handle == uint32.MaxValue || Index == uint32.MaxValue)
				return false;

			return IsValidSubMesh(ref Handle);
		}
	}

	public static class Gfx
	{
		static float4x4 g_mView, g_mProj, g_mViewProj, g_mWorld, g_mWorldViewProj;

		[CLink]
		public static extern bool SetupGfx(GfxSetup* setup);

		[CLink]
		public static extern TextureHandle LoadTexture(char8* path);

		[CLink]
		public static extern void SetClearColor(ref Color _color);

		[CLink]
		public static extern void BeginDefaultPass();

		[CLink]
		public static extern void EndPass();

		[CLink]
		public static extern void Commit();

		[CLink]
		public static extern void Present();

		[CLink]
		public static extern void SetWindowSize(int32 _width, int32 _height, bool _soft);

		[CLink]
		public static extern void GetViewportSize(out RectI _rect);

		public static void SetViewMatrix(in float4x4 _view)
		{
			g_mView = _view;
			g_mViewProj = g_mProj * g_mView;
		}

		public static float4x4 GetViewMatrix()
		{
			return g_mView;
		}

		//------------------------------------------------------------------------------
		public static void SetProjMatrix(in float4x4 _proj)
		{
			g_mProj = _proj;
			g_mViewProj = g_mProj * g_mView;
		}

		//------------------------------------------------------------------------------
		public static float4x4 GetProjMatrix()
		{
			return g_mProj;
		}

		public static void SetWorldMatrix(in float4x4 _world)
		{
			g_mWorld = _world;
			g_mWorldViewProj = g_mViewProj * g_mWorld;
		}

		public static float4x4 GetWorldMatrix()
		{
			return g_mWorld;
		}

		//------------------------------------------------------------------------------
		public static void SetViewProjMatrix(in float4x4 _view, in float4x4 _proj)
		{
			g_mView = _view;
			g_mProj = _proj;
			g_mViewProj = g_mProj * g_mView;
		}

		//------------------------------------------------------------------------------
		public static float4x4 GetViewProjMatrix()
		{
			return g_mViewProj;
		}

		public static void SetWorldViewProjMatrix(in float4x4 _world, in float4x4 _view, in float4x4 _proj)
		{
			SetViewProjMatrix(_view, _proj);
			SetWorldMatrix(_world);
		}

		public static float4x4 GetWorldViewProjMatrix()
		{
			return g_mWorldViewProj;
		}

	}
}
