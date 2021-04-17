using System;
using ari.io;

namespace ari
{
	static
	{
		[Export, CLink, AlwaysInclude]
		static GfxSetup* GetGfxSetup()
		{
			if (g_app == null)
				return null;
			return g_app.GetGfxSetup();
		}

		[Export, CLink, AlwaysInclude]
		static void OnInit()
		{
			// Init timer
			sokol.Time.setup();

			// get the app setup configs
			Gfx.SetupGfx(GetGfxSetup());

			g_app.OnInit();
			LastTime = sokol.Time.now();
		}

		[Export, CLink, AlwaysInclude]
		static void OnFrame()
		{
			FrameNumber += 1;

			// update Io threads
			Io.UpdateIo();

			// calculate the elapsed time
			var elapsed = (float)sokol.Time.sec(sokol.Time.laptime(&LastTime));

			// Update the app
			g_app.OnFrame(elapsed);

		}


		[Export, CLink, AlwaysInclude]
		static void OnCleanUp()
		{
			g_app.OnCleanup();
		}

		[Export, CLink, AlwaysInclude]
		static void OnEvent(sokol.App.Event* _event)
		{
			WindowHandle h;
			h.Handle = h.Index = 0;
			g_app.OnEvent((ari_event*)_event, ref h);
		}

		[Export, CLink, AlwaysInclude]
		static void OnFail(char8* errMsg)
		{
			ari.core.Logger.Error(errMsg);
		}

		static uint64 FrameNumber;

		static Application g_app = null;

		public static void OnEvent(ari_event* _event, ref WindowHandle _handle)
		{
			g_app.OnEvent(_event, ref _handle);
		}

		public static uint64 LastTime;

		public static void RunApplication(Application _app)
		{
			g_app = _app;
			// Init SAPP
			sokol.App.Desc desc = .();
			desc.init_cb = => OnInit;
			desc.frame_cb = => OnFrame;
			desc.event_cb = => OnEvent;
			desc.cleanup_cb = => OnCleanUp;
			desc.fail_cb = => OnFail;
			GfxSetup* setup = _app.GetGfxSetup();
			desc.window_title = setup.window.Title == null ? "Ari 0.8" : setup.window.Title;
			desc.width = setup.window.Width;
			desc.height = setup.window.Height;
			desc.high_dpi = setup.window.HighDpi;
			desc.sample_count = setup.sample_count;
			desc.swap_interval = setup.swap_interval;
			desc.fullscreen = setup.window.FullScreen;

			sokol.App.run(&desc);

			/*
#if BF_PLATFORM_ANDROID
			while(Io.Run())
			{
				System.Threading.Thread.Sleep(1000);
			}
			return;
#else // !BF_PLATFORM_ANDROID

			// init the app
			_app.OnInit();

			// get time
			LastTime = Timer.Now();

			// bind on event
			SetOnEventCallBack(=> OnEvent);

			// run the loop
			while (!Application.Exit && Io.Run())
			{
				FrameNumber += 1;

				// update Io threads
				Io.UpdateIo();

				// calculate the elapsed time
				double elapsed = Timer.ToSeconds(Timer.LapTime(ref LastTime));

				// Update the app
				_app.OnFrame((float)elapsed);

#if ARI_SERVER
				// Limit the fps to 60
				double sleepTime = 0.0166666667 - Timer.ToSeconds(Timer.Since(LastTime)); 
				uint64 LastTime2 = LastTime;
				while (sleepTime > 0)
				{
					System.Threading.Thread.Sleep(1);
					sleepTime -= Timer.ToSeconds(Timer.LapTime(ref LastTime2));
				}
#endif
			}

			// clean up the app
			_app.OnCleanup();
#endif // !BF_PLATFORM_ANDROID
			*/
		}
	}

	public class Application
	{
		protected GfxSetup* setup = null;

		public static bool Exit = false;

		public virtual GfxSetup* GetGfxSetup()
		{
			if (setup == null)
				setup = new GfxSetup();
			return setup;
		}

		public virtual void OnInit()
		{

		}

		public virtual void OnFrame(float _elapsedTime)
		{

		}

		public virtual void OnCleanup()
		{
			delete setup;
		}

		public virtual void OnEvent(ari_event* _event, ref WindowHandle _handle)
		{

		}
	}
}
