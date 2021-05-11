using System;
using System.Threading;
using ari.core;

namespace testEN
{
	class TestMpScQueue
	{
		const int _writers_count = 64;
		Thread[] _writerThreads = null ~ delete _;
		Thread _readerThread = null;
		int32 _i = 0;
		MpScQueue<int32> _queue = new MpScQueue<int32>() ~ delete _;

		void WriteThread()
		{
			while (true)
			{
				var i = Interlocked.Add(ref _i, 1, .Acquire);
				if (i > 100000)
					return;
				_queue.Push(ref i);
			}
		}

		void ReadThread()
		{
			int32 i = 0;
			int32 c = 2;
			while(c < 100000)
			{
				if (_queue.TryPop(ref i))
				{
					//if (i != c)
					{
						Console.Write(i);
						Console.Write("\t");
					}
					c++;
				}
			}
		}

		public void Run()
		{
			_writerThreads = new Thread[_writers_count];
			
			_readerThread = new Thread(new => ReadThread);
			_readerThread.Start();

			Thread.Sleep(20);

			for (int i = 0; i < _writers_count; i++)
			{
				_writerThreads[i] = new Thread(new => WriteThread);
				_writerThreads[i].Start(true);
			}

			_readerThread.Join();
		}
	}
}
