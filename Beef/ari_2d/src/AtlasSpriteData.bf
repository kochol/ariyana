using System;
using System.Collections;

namespace ari.en
{
	[Reflect, AlwaysInclude(AssumeInstantiated=true, IncludeAllMethods=true)]
	class AtlasSpriteData
	{
		public String name = null ~ delete _;

		public List<int> size = null ~ delete _;

		public List<int> sprite_rect = null ~ delete _;

		public List<int> sheet_rect = null ~ delete _;

	}
}
