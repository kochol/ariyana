using ari;

namespace projectmanager
{
	class ProjMgrApp: Application
	{
		public this()
		{
			setup = new GfxSetup();
			setup.window.Width = 800;
			setup.window.Height = 600;
			setup.window.HighDpi = false;
			setup.swap_interval = 1;
		}

		public ~this()
		{

		}

		public override void OnInit()
		{
			base.OnInit();
		}

		public override void OnFrame(float _elapsedTime)
		{
			base.OnFrame(_elapsedTime);
		}

		public override void OnEvent(ari_event* _event, ref ari.io.WindowHandle _handle)
		{
		}

		public override void OnCleanup()
		{
			base.OnCleanup();
		}
	}
}
