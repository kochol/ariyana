using System;

namespace ari
{
	public class RenderSystem : AriSystem
	{
		[CLink]
		static extern void* CreateRenderSystem();

		public this()
		{
			_obj = CreateRenderSystem();
		}
	}
}
