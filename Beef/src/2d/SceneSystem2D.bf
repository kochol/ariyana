using System;

namespace ari
{
	public class SceneSystem2D : AriSystem
	{
		[CLink]
		static extern void* CreateSceneSystem2D();

		public this()
		{
			_obj = CreateSceneSystem2D();
		}
	}
}
