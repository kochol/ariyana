using System;

namespace ari
{
	public class AriSystem
	{
		protected void* _obj = null;

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
		protected virtual void Configure(World _world)
		{

		}
	}
}
