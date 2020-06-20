using System;

namespace ari.core
{
	public class SpScQueue<T> 
	{
		void* queue = null;
		bool is_object;

		[CLink]
		static extern void* ari_queue_spsc_create(int32 item_sz, int32 capacity);

		public this(int32 _capacity)
		{
			int32 size;
			if (typeof(T).IsObject)
			{
				size = sizeof(int);
				is_object = true;
			}
			else
			{
				size = sizeof(T);
				is_object = false;
			}

			queue = ari_queue_spsc_create(size, _capacity);
		}

		public this(): this(16)
		{
			
		}

		[CLink]
		static extern void ari_queue_spsc_destroy(void* queue);

		public ~this()
		{
			ari_queue_spsc_destroy(queue);
			queue = null;
		}

		[CLink]
		static extern bool ari_queue_spsc_produce(void* queue, void* data);

		[CLink]
		static extern bool ari_queue_spsc_full(void* queue);

		[CLink]
		static extern bool ari_queue_spsc_grow(void* queue);

		public void Push(ref T item)
		{
			if (!ari_queue_spsc_produce(queue, (void*)&item))
			{
				Runtime.Assert(ari_queue_spsc_grow(queue), "Can not grow the queue. out of memory");
				Runtime.Assert(ari_queue_spsc_produce(queue, (void*)&item), "Can't add item to queue");
			}
		}

		[CLink]
		static extern bool ari_queue_spsc_consume(void* queue, void* data);

		public bool TryPop(ref T item)
		{
			if (is_object)
			{
				uint p = 0;
				bool b = ari_queue_spsc_consume(queue, (void*)&p);
				item = (T)Internal.UnsafeCastToObject((void*)p);
				return b;
			}	
			return ari_queue_spsc_consume(queue, (void*)&item);
		}
	}
}
