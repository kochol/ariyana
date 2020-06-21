using System.Threading;
using System;

namespace ari.net
{
	class HttpClientService: AriSystem
	{
		struct ResonseTuple
		{
			public OnRequestDoneDelegate OnDone;
			public HttpResponse Response;
		}

		// Session
		curl.Session session = new curl.Session();

		// Thread stuff
		private ThreadStart WorkerThreadDelegate = null;
		private Thread WorkerThreadObj = null;
		private WaitEvent ThreadWaitEvent = new WaitEvent();
		private bool ThreadRun = true;
		private ari.core.SpScQueue<HttpRequest> request_queue = new ari.core.SpScQueue<HttpRequest>();
		private ari.core.SpScQueue<ResonseTuple> response_queue = new ari.core.SpScQueue<ResonseTuple>(); 

		private void WorkerThread()
		{
			while(ThreadRun)
			{
				if (ThreadWaitEvent.WaitFor())
				{
					HttpRequest r = .();
					while (request_queue.TryPop(ref r))
					{
						session.Url = r.Url;
						HttpResponse res = .();
						res.Body = session.GetString();
						ResonseTuple t = .();
						t.Response = res;
						t.OnDone = r.OnRequestDone;
						response_queue.Push(ref t);
					}
				}
			}
			delete ThreadWaitEvent;
			ThreadWaitEvent = null;
		}

		public this()
		{
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
			delete response_queue;
			delete session;
		}

		protected override void Update(World _world, float _elapsed)
		{
			base.Update(_world, _elapsed);
			ResonseTuple r = .();
			while (response_queue.TryPop(ref r))
			{
				Console.WriteLine(r.Response.Body);
				if (r.OnDone != null)
				{
					r.OnDone(r.Response);
					delete r.OnDone;
				}
			}
		}

		public void AddRequest(ref HttpRequest request)
		{
			request_queue.Push(ref request);
			ThreadWaitEvent.Set();
		}
	}
}
