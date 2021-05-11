using System;
using System.Threading;

namespace ari.core
{
	// Inspired from https://github.com/dbittman/waitfree-mpsc-queue project
	class MpScQueue<T>
	{
		class MpScQueueBin
		{
			public int32 Capacity => _max;
			public int32 Count => _count;
			public MpScQueueBin Next = null ~ delete _;
			public MpScQueueBin Prev;

			int32 _count;
			int32 _head;
			int32 _tail;
			int32 _max;
			T[] _buff;

			[AllowAppend]
			public this(int32 capacity = 16, MpScQueueBin prev = null)
			{
				let _capacity = (((capacity) + (15)) & ((~0) & (~(15)))); // align & mask to 15

				let buff = append T[_capacity];
				_max = _capacity;
				_buff = buff;
				Prev = prev;
				Interlocked.Fence(.Release);
			}

			public bool Push(ref T item, out bool reset)
			{
				reset = false;
				let count = Interlocked.Add(ref _count, 1, .Acquire);
				if (count >= _max)
				{
					// Queue is full
					Interlocked.Sub(ref _count, 1, .Release);
					return false;
				}

				/* increment the head, which gives us 'exclusive' access to that element */
				let head = Interlocked.Add(ref _head, 1, .Acquire);
				Runtime.Assert(_buff[head % _max] == default);
				let rv = Interlocked.Exchange(ref _buff[head % _max], item, .Release);
				Runtime.Assert(rv == default);

				let tail = Volatile.Read(ref _tail);
				if (tail >= _max)
				{
					reset = true;
					Volatile.Write(ref _tail, 0);
				}

				return true;
			}

			public bool TryPop(ref T item)
			{
				let tail = Volatile.Read(ref _tail);
				if (tail >= _max)
					return false;

				let ret = Interlocked.Exchange(ref _buff[tail], default, .Acquire);
				if (ret == default)
					return false;

				Interlocked.Add(ref _tail, 1, .Acquire);
				let r = Interlocked.Sub(ref _count, 1, .Release);
				Runtime.Assert(r > 0);
				return true;
			}

		} // MpScQueueBin

		MpScQueueBin _head;
		MpScQueueBin _pop_data;
		MpScQueueBin _push_data;
		Monitor _monitor = new Monitor() ~ delete _;

		public this(int32 capacity = 16)
		{
			let data = new MpScQueueBin(capacity);
			_head = data;
			_pop_data = data;
			_push_data = data;
		}

		void PushBinReseted()
		{
			_monitor.Enter();
			defer _monitor.Exit();

			// move current push bin to the last
			if (_push_data.Next == null)
				return;
			var last_bin = _head;
			while (last_bin.Next != null)
			{
				last_bin = last_bin.Next;
			}
			if (_push_data.Prev == null)
			{
				Volatile.Write(ref _head, _push_data.Next);
				Volatile.Write(ref _push_data.Next.Prev, null);
				Volatile.Write(ref _push_data.Next, null);
				Volatile.Write(ref _push_data.Prev, last_bin);
				Volatile.Write(ref last_bin.Next, _push_data);
				return;
			}
			Volatile.Write(ref _push_data.Prev.Next, _push_data.Next);
			Volatile.Write(ref _push_data.Next.Prev, _push_data.Prev);
			Volatile.Write(ref _push_data.Next, null);
			Volatile.Write(ref _push_data.Prev, last_bin);
			Volatile.Write(ref last_bin.Next, _push_data);
		}

		public void Push(ref T item)
		{
			var data = Volatile.Read(ref _push_data);
			var last_data = data;
			bool reset;
			int32 maxCap = 16;
			while (data != null)
			{
				if (data.Push(ref item, out reset))
				{
					Volatile.Write(ref _push_data, data);
					if (reset)
						PushBinReseted();
					return;
				}
				let data_next = Volatile.Read(ref data.Next);
				if (data_next == null)
					last_data = data;
				if (data.Capacity > maxCap)
					maxCap = data.Capacity;
				data = data_next;
			}
			data = _head;
			while (data != _push_data)
			{
				if (data.Push(ref item, out reset))
				{
					Volatile.Write(ref _push_data, data);
					if (reset)
						PushBinReseted();
					return;
				}
				let data_next = Volatile.Read(ref data.Next);
				if (data.Capacity > maxCap)
					maxCap = data.Capacity;
				data = data_next;
			}

			// Create a new bin
			Volatile.Write(ref last_data.Next, new MpScQueueBin(maxCap * 2, last_data));
			last_data.Next.Push(ref item, out reset);
			_push_data = last_data.Next;
		}

	} // MpScQueue<T>
}
