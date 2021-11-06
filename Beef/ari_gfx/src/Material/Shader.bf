using System;
using System.Collections;

namespace ari.gfx.Material
{
	class Shader
	{
		public String cmdline;

		public List<ShaderFile> files;

		public List<Technique> techniques;

		public List<ShaderFailure> failures;
	}
}
