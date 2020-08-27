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
		public curl.Session session = new curl.Session();

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

						// set the headers
						if (r.[Friend]SetHeaders)
						{
							session.SetHeaders(r.[Friend]Headers);
						}

						// Set the verb
						session.SetVerb(r.Verb);

						// set the file upload
						if (r.FileSize > 0)
						{
							session.AddFileToUpload(r.FileData, r.FileSize);
						}
						if (r.Body != null)
						{
							session.SetRequestBody(r.Body);
						}

						let sr = session.GetString();

						if (r.FileSize > 0)
						{
							// reset upload data
							session.AddFileToUpload(null, 0);
						}

						if (r.OnRequestDone == null) // The request response is not important for the user
						{
							r.Dispose();
							switch(sr)
							{
							case .Err(let err):
								let s = scope String();
								err.ToString(s);
								Logger.Error(s);
							case .Ok(let val):

							}
							continue;
						}

						HttpResponse res = .();
						switch (sr)
						{
						case .Err(let err):
							res.Status = err;
						case .Ok(let val):
							res.Body = new String(val);
							res.Status = .Ok;
							res.StatusCode = session.ResponseCode;
						}
						ResonseTuple t = .();
						t.Response = res;
						t.OnDone = r.OnRequestDone;
						response_queue.Push(ref t);
						r.Dispose();
					}
				}
			}
			delete ThreadWaitEvent;
			ThreadWaitEvent = null;
		}

		public this()
		{
			session.[Friend]easy.SetOpt(.SSLVerifyPeer, false);
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
				Logger.Debug(r.Response.Body);
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
