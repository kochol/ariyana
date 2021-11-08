using System;
using ari.core;
using ari.gfx.Material;

namespace ari.gfx
{
	public class PmfxShaderLoader : IResourceLoader
	{
		public bool CanLoadResource(String path)
		{
			return path.EndsWith(".pmfx");
		}

		public T LoadResource<T>(String path) where T : Resource
		{
			Shader r = new Shader();

			return r;
		}
	}
}
