using System;
using System.Collections;

namespace ari.en
{
	[Reflect]
	class AtlasData
	{
		public String image = null ~ delete _;

		public float image_width;

		public float image_height;

		public List<AtlasSpriteData> sprites = null ~ DeleteContainerAndItems!(_);
	}
}
