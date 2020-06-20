using System;
using System.Threading;

namespace ari.user
{
	public class ProfileSystem : AriSystem
	{
		struct Request
		{
			public String Url;
			public String Result;
		}

		private String ServerAddress = null ~ delete _;
		private String Token = new String() ~ delete _;

		// Session
		curl.Session session = new curl.Session() ~ delete _;

		// Thread stuff
		private ThreadStart WorkerThreadDelegate = null;
		private Thread WorkerThreadObj = null;
		private WaitEvent ThreadWaitEvent = new WaitEvent();
		private bool ThreadRun = true;
		private ari.core.SpScQueue<Request> request_queue = new ari.core.SpScQueue<Request>();
		private ari.core.SpScQueue<Request> response_queue = new ari.core.SpScQueue<Request>() ~ delete _; 

		private void WorkerThread()
		{
			while(ThreadRun)
			{
				if (ThreadWaitEvent.WaitFor())
				{
					Request r = .();
					while (request_queue.TryPop(ref r))
					{
						session.Url = r.Url;
						r.Result = session.GetString();
						response_queue.Push(ref r);
					}
				}
			}
			delete ThreadWaitEvent;
			ThreadWaitEvent = null;
		}

		public this(String server_address)
		{
			ServerAddress = new String(server_address);
			WorkerThreadDelegate = new => WorkerThread;
			WorkerThreadObj = new Thread(WorkerThreadDelegate);
			WorkerThreadObj.Start();
			WorkerThreadObj.SetName("ProfileServer");
		}

		public ~this()
		{
			ThreadRun = false;
			ThreadWaitEvent.Set();
			while (ThreadWaitEvent != null)
			{
				Thread.Sleep(1);
			}
			delete request_queue;
		}

		protected override void Update(World _world, float _elapsed)
		{
			base.Update(_world, _elapsed);
			Request r = .();
			while (response_queue.TryPop(ref r))
			{
				Console.WriteLine(r.Result);
			}
		}

		public void Login()
		{
			Request login = .();
			login.Url = new String(ServerAddress);
			login.Url.AppendF("auth/{}/", Io.GetDeviceID());
#if BF_PLATFORM_WINDOWS
			login.Url.Append("Windows/");
#elif BF_PLATFORM_LINUX
			login.Url.Append("Linux/");
#elif BF_PLATFORM_ANDROID
			login.Url.Append("Android/");
#endif
			let os = scope OperatingSystem();
			os.ToString(login.Url);

			request_queue.Push(ref login);
			ThreadWaitEvent.Set();
		}
	}
}
