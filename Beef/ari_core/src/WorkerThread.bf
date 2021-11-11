using System.Threading;
using System;
namespace ari.core
{
	public abstract class WorkerThread
	{
		// Thread stuff
		private ThreadStart WorkerThreadDelegate = null;
		private Thread WorkerThreadObj = null;
		protected WaitEvent ThreadWaitEvent = new WaitEvent();
		protected bool ThreadRun = true;

		public this(String threadName)
		{
			WorkerThreadDelegate = new => WorkerThreadFunction;
			WorkerThreadObj = new Thread(WorkerThreadDelegate);
			WorkerThreadObj.Start();
			WorkerThreadObj.SetName(threadName);
		}

		public ~this()
		{
			ThreadRun = false;
			ThreadWaitEvent.Set();
			while (ThreadWaitEvent != null)
			{
				Thread.Sleep(1);
			}
		}

		public abstract void OnWorkerUpdate();

		private void WorkerThreadFunction()
		{
			while(ThreadRun)
			{
				if (ThreadWaitEvent.WaitFor())
				{
					OnWorkerUpdate();
				}
			}
			delete ThreadWaitEvent;
			ThreadWaitEvent = null;
		}
	}
}
