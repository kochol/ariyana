using System;
using System.Threading;

namespace ari.user
{
	public class ProfileServer
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
		private ThreadStart WorkerThreadDelegate = null ~ delete _;
		private Thread WorkerThreadObj = null ~ delete _;
		private WaitEvent ThreadWaitEvent = new WaitEvent() ~ delete _;
		private bool ThreadRun = true;
		private ari.core.SpScQueue<Request> request_queue = new ari.core.SpScQueue<Request>() ~ delete _; 

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
					}
				}
			}
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
			//os.ToString(login.Url);
			login.Url.Append("WDW");

			request_queue.Push(ref login);
			ThreadWaitEvent.Set();
		}
	}
}
