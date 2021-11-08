using System;
using System.Collections;
using ari.core;

namespace ari.gfx.Material
{
	class Shader : Resource
	{
		public String cmdline;

		public List<ShaderFile> files;

		public List<Technique> techniques;

		public List<ShaderFailure> failures;
	}
}
