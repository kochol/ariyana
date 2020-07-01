using System;

namespace ari
{
	public class AriSystem
	{
		protected void* _obj = null;
		protected bool IsNativeSystem = false;

		[CLink]
		static extern void DeleteSystem(void* _obj);

		public ~this()
		{
			if (_obj != null)
			{
				DeleteSystem(_obj);
				_obj = null;
			}
		}

		// World call this on configure
		protected virtual void Configure(World _world) { }

		protected virtual void Unconfigure(World _world) { }

		protected virtual void Update(World _world, float _elapsed) { }
	}
}
