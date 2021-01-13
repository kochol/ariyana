using System;

namespace ari.en
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

		[CLink]
		static extern void AddChildSystem(void* _world, void* _obj, void* _system);

		public void AddChild(World world, AriSystem system)
		{
			if (IsNativeSystem)
				AddChildSystem(world.[Friend]_obj, _obj, system._obj);

			system.Configure(world);
		}
	}
}
