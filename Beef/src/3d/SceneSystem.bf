using System;

namespace ari
{
	public class SceneSystem : AriSystem
	{
		[CLink]
		static extern void* CreateSceneSystem();

		public this()
		{
			_obj = CreateSceneSystem();
		}
	}
}
