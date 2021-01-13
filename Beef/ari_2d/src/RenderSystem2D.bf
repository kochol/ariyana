using System;

namespace ari.en
{
	public class RenderSystem2D : AriSystem
	{
		[CLink]
		static extern void* CreateRenderSystem2D();

		public this()
		{
			_obj = CreateRenderSystem2D();
			IsNativeSystem = true;
		}
	}
}
