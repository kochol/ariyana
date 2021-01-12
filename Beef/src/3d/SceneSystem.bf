using System;

namespace ari.en
{
	public class SceneSystem : AriSystem
	{
		[CLink]
		static extern void* CreateSceneSystem();

		public this()
		{
			_obj = CreateSceneSystem();
			IsNativeSystem = true;
		}
	}
}
