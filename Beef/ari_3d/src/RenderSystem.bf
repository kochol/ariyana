using System;

namespace ari.en
{
	public class RenderSystem : AriSystem
	{
		[CLink]
		static extern void* CreateRenderSystem();

		public this()
		{
			_obj = CreateRenderSystem();
			IsNativeSystem = true;
		}
	}
}
