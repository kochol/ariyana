using System;

namespace ari.gui
{
	class Window : Gui
	{
		public char8** WindowName;
		public bool* CloseButton;
		public bool* IsOpen;
		public Vector2*	Pos;
		public Vector2*	Size;
		public int*	Flags;

		public this(GuiHandle _handle): base (_handle)
		{

		}

		[CRepr]
		struct WindowMembers
		{
			public char8** WindowName;
			public bool* CloseButton;
			public bool* IsOpen;
			public Vector2*	Pos;
			public Vector2*	Size;
			public int*	Flags;
		}

		[CLink]
		static extern WindowMembers GetWindowMembers(void* _node);

		protected override void Init()
		{
			base.Init();
			var m = GetWindowMembers(Handle._obj);
			WindowName = m.WindowName;
			CloseButton = m.CloseButton;
			IsOpen = m.IsOpen;
			Pos = m.Pos;
			Size = m.Size;
			Flags = m.Flags;
		}
	}
}
