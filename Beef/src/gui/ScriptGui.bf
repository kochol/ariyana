using System;

namespace ari.gui
{
	class ScriptGui: Gui
	{
		protected World _world = null;

		[CLink]
		static extern GuiHandle CreateScriptGuiComponent();

		public this(): base(CreateScriptGuiComponent())
		{
			Init();
		}

		public this(GuiHandle _handle): base(_handle)
		{

		}

		public ~this()
		{
			InitScriptGuiMembers(Handle._obj, null, null, null);
		}

		function bool sgBeginRender(void* _userData);
		function void sgEndRender(void* _userData);

		[CLink]
		static extern void InitScriptGuiMembers(void* _node, void* _userData, sgBeginRender OnBegineRender, sgEndRender OnEndRender);

		static bool cbBeginRender(void* _userData)
		{
			return ((ScriptGui)Internal.UnsafeCastToObject(_userData)).BeginRender();
		}

		static void cbEndRender(void* _userData)
		{
			((ScriptGui)Internal.UnsafeCastToObject(_userData)).EndRender();
		}

		protected override void Init()
		{
			base.Init();
			InitScriptGuiMembers(Handle._obj, Internal.UnsafeCastToPtr(this),
				=> cbBeginRender, => cbEndRender);
		}

		protected virtual bool BeginRender()
		{
			return false;
		}

		protected virtual void EndRender()
		{

		}
	}
}
