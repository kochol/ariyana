using System;

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
		[CLink]
		public static extern bool SetupGfx(GfxSetup* setup);

		[CLink]
		public static extern TextureHandle LoadTexture(char8* path);

		[CLink]
		public static extern void SetClearColor(ref Color _color);
	}
}
