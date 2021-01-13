using System;

namespace ari.en
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
		static extern void GetCanvasMembers(void* _node, out CanvasMembers _out);

		protected override void Init()
		{
			base.Init();
			CanvasMembers m;
			GetCanvasMembers(handle._obj, out m);
			Rect = m.Rect;
		}
	}
}
