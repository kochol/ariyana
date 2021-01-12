using System;
using ari.net;

namespace ari.en
{
	extension World
	{
		[CLink]
		static extern PropertyReplicatorHandle CreatePropertyReplicatorComponent();

		public static PropertyReplicator CreatePropertyReplicator()
		{
			return new PropertyReplicator(CreatePropertyReplicatorComponent());
		}

		[CLink]
		static extern void AddPropertyReplicatorToWorld(void* _world, ref EntityHandle _entity, ref PropertyReplicatorHandle _cmp);

		public void AddComponent(Entity _entity, PropertyReplicator _pr)
		{
			AddPropertyReplicatorToWorld(_obj, ref _entity.Handle, ref _pr.[Friend]handle);
		}
	}
}
