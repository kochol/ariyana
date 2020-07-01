using System;

namespace ari.gui
{
	class GuiSystem : AriSystem
	{
		[CLink]
		static extern void* CreateGuiSystem();

		public this()
		{
			_obj = CreateGuiSystem();
			IsNativeSystem = true;
		}
	}
}
