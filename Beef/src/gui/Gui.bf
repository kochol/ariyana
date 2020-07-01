using System;

namespace ari.gui
{
	[CRepr]
	struct GuiHandle
	{	
		public uint32 Handle = uint32.MaxValue;
		public uint32 Index = uint32.MaxValue;
		public void* _obj = null;
		public Entity Owner = null;

		[CLink]
		static extern bool IsValidGui(ref uint32 _handle);

		bool IsValid() mut
		{
			if (Handle == uint32.MaxValue || Index == uint32.MaxValue)
				return false;

			return IsValidGui(ref Handle);
		}
	}

	class Gui : Node
	{
		public bool* SameLine;
		public bool* Separator;
		public bool* Visible;

		public GuiHandle Handle;

		// constructor
		public this(GuiHandle _handle) : base(_handle._obj)
		{
			this.Handle = _handle;
		}

		[CRepr]
		struct GuiMembers
		{
			public bool* SameLine;
			public bool* Separator;
			public bool* Visible;
		}

		[CLink]
		static extern GuiMembers GetGuiMembers(void* _node);

		// Init the members
		protected virtual void Init()
		{
			Runtime.Assert(Handle._obj != null);
			var m = GetGuiMembers(Handle._obj);
			SameLine = m.SameLine;
			Separator = m.Separator;
			Visible = m.Visible;
		}
	}
}
