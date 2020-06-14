using System;

namespace ari
{
	public class RenderSystem2D : AriSystem
	{
		[CLink]
		static extern void* CreateRenderSystem2D();

		public this()
		{
			_obj = CreateRenderSystem2D();
		}
	}
}
