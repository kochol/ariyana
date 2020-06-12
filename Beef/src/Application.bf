using System;

namespace ari
{

	function void OnEventCB(ari_event* _event, ref WindowHandle _handle);

	static
	{
		[Export, CLink, AlwaysInclude]
		static GfxSetup* GetGfxSetup()
		{
			return g_app.GetGfxSetup();
		}

		[Export, CLink, AlwaysInclude]
		static void OnInit()
		{
			g_app.OnInit();
			LastTime = Timer.Now();
		}

		[Export, CLink, AlwaysInclude]
		static void OnFrame()
		{
			*FrameNumber += 1;

			// update Io threads
			Io.UpdateIo();

			// calculate the elapsed time
			float elapsed = (float)Timer.ToSeconds(Timer.LapTime(ref LastTime));

			// Update the app
			g_app.OnFrame(elapsed);

		}


		[Export, CLink, AlwaysInclude]
		static void OnCleanUp()
		{
			g_app.OnCleanup();
		}

		[Export, CLink, AlwaysInclude]
		static void OnEvent(ari_event* _event)
		{
			WindowHandle h;
			h.Handle = h.Index = 0;
			g_app.OnEvent(_event, ref h);
		}

		[Export, CLink, AlwaysInclude]
		static void OnFail()
		{
			
		}

		[CLink]
		static extern uint64* GetFrameNumberPointer();

		[CLink]
		static extern void SetOnEventCallBack(OnEventCB _event_cb);

		static uint64* FrameNumber;

		static Application g_app = null;

		public static void OnEvent(ari_event* _event, ref WindowHandle _handle)
		{
			g_app.OnEvent(_event, ref _handle);
		}

		public static uint64 LastTime;

		public static void RunApplication(Application _app)
		{
			FrameNumber = GetFrameNumberPointer();
			g_app = _app;

			// Init timer
			Timer.Init();

			// get the app setup configs
			Gfx.SetupGfx(_app.GetGfxSetup());

#if BF_PLATFORM_ANDROID
			while(Io.Run())
			{
				System.Threading.Thread.Sleep(1000);
			}
#else // !BF_PLATFORM_ANDROID
			// init the app
			_app.OnInit();

			// get time
			LastTime = Timer.Now();

			// bind on event
			SetOnEventCallBack(=> OnEvent);

			// run the loop
			while (Io.Run())
			{
				*FrameNumber += 1;

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
		}
	}

	public class Application
	{
		protected GfxSetup* setup = null;

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
