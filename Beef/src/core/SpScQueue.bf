using System;
using System.Threading;

namespace ari.core
{
public class SpScQueue<T> 
	{
		class SpScQueueBin
		{
			public int Capacity;
			public SpScQueueBin Next = null ~ delete _;
			public SpScQueueBin Prev;

			int _iter_push;
			int _iter_pop;
			T[] _buff;

			[AllowAppend]
			public this(int capacity = 16, SpScQueueBin prev = null)
			{
				let _capacity = (((capacity) + (15)) & ((~0) & (~(15)))); // align & mask to 15

				let buff = append T[_capacity];
				Capacity = _capacity;
				_buff = buff;
				_iter_pop = _iter_push = Capacity;
				Prev = prev;
			}

			public bool Push(ref T item, out bool reset)
			{
				reset = false;
				if (_iter_push > 0)
				{
					_buff[--_iter_push] = item;
					return true;
				}
				else
				{
					let iter_pop =	Volatile.Read(ref _iter_pop);
					if (iter_pop == 0)
					{
						reset = true;
						_iter_push = Capacity;
						_buff[--_iter_push] = item;
						_iter_pop = Capacity;
						return true;
					}
				}

				return false;
			}

			public bool TryPop(ref T item)
			{
				if (_iter_pop > 0)
				{
					let iter_push =	Volatile.Read(ref _iter_push);
					if (_iter_pop > iter_push)
					{
						item = _buff[--_iter_pop];
						return true;
					}
				}

				return false;
			}
		} // class SpScQueueBin

		SpScQueueBin _data;
		SpScQueueBin _pop_data;
		SpScQueueBin _push_data;

		public this(int32 capacity = 16)
		{
			let data = new SpScQueueBin(capacity);
			_data = data;
			_pop_data = data;
			_push_data = data;
		}

		public ~this()
		{
			delete _data;
		}

		void PushBinReseted()
		{
			// move current push bin to the last
			if (_push_data.Next == null)
				return;
			var last_bin = _data;
			while (last_bin.Next != null)
			{
				last_bin = last_bin.Next;
			}
			if (_push_data.Prev == null)
			{
				Volatile.Write(ref _data, _push_data.Next);
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
			var data = _push_data;
			var last_data = data;
			bool reset;
			int maxCap = 0;
			while (data != null)
			{
				if (data.Push(ref item, out reset))
				{
					_push_data = data;
					if (reset)
						PushBinReseted();
					return;
				}
				if (data.Next == null)
					last_data = data;
				if (data.[Friend]Capacity > maxCap)
					maxCap = data.[Friend]Capacity;
				data = data.Next;
			}
			data = _data;
			while (data != _push_data)
			{
				if (data.Push(ref item, out reset))
				{
					_push_data = data;
					if (reset)
						PushBinReseted();
					return;
				}
				if (data.[Friend]Capacity > maxCap)
					maxCap = data.[Friend]Capacity;
				data = data.Next;
			}
			
			// Create a new bin
			Volatile.Write(ref last_data.Next, new SpScQueueBin(maxCap * 2, last_data));
			last_data.Next.Push(ref item, out reset);
			_push_data = last_data.Next;
		}

		public bool TryPop(ref T item)
		{
			var data = _pop_data;
			while (data != null)
			{
				if (data.TryPop(ref item))
				{
					_pop_data = data;
					return true;
				}
				if (data.[Friend]_iter_pop == 0)
				{
					data = data.Next;
				}
				else
					break;
			}
			data = Volatile.Read(ref _data);
			while (data != _pop_data)
			{
				if (data.TryPop(ref item))
				{
					_pop_data = data;
					return true;
				}
				data = data.Next;
			}
			return false;
		}
	}
}
