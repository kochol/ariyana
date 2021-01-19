using System;

namespace ari.gui
{
	class Window : Gui
	{
		char8** p_window_name;
		public bool* CloseButton;
		public bool* IsOpen;
		public Vector2*	Pos;
		public Vector2*	Size;
		public int32* Flags;
		String window_name = null;

		public this(GuiHandle _handle, String _window_name): base (_handle)
		{
			window_name = _window_name;
		}

		public String WindowName {
			get
			{
				return window_name;
			}
			set
			{
				window_name = value;
				*p_window_name = window_name;
			}
		}

		[CRepr]
		struct WindowMembers
		{
			public char8** WindowName;
			public bool* CloseButton;
			public bool* IsOpen;
			public Vector2*	Pos;
			public Vector2*	Size;
			public int32*	Flags;
		}

		[CLink]
		static extern WindowMembers GetWindowMembers(void* _node);

		protected override void Init()
		{
			base.Init();
			var m = GetWindowMembers(Handle._obj);
			p_window_name = m.WindowName;
			*p_window_name = window_name;
			CloseButton = m.CloseButton;
			IsOpen = m.IsOpen;
			Pos = m.Pos;
			Size = m.Size;
			Flags = m.Flags;
		}
	}
}
