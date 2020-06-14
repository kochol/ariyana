using System;

namespace ari
{
	public class Canvas : Node2D
	{
		public RectI* Rect;

		public this(Node2dHandle _handle) : base(_handle)
		{

		}

		[CRepr]
		struct CanvasMembers
		{
			 public RectI* Rect;
		}

		[CLink]
		static extern CanvasMembers GetCanvasMembers(void* _node);

		protected override void Init()
		{
			base.Init();
			var m = GetCanvasMembers(handle._obj);
			Rect = m.Rect;
		}
	}
}
