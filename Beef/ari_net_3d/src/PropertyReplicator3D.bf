using System;
using ari.en;

namespace ari.net
{
	extension PropertyReplicator
	{
		[CLink]
		static extern void AddNode3dProperty(void* _obj, ref Node3dHandle _node, char8* _propertyName);

		// This function can replicate Position, Rotation and Scale
		public void AddProperty(Node3D _box, char8* _name)
		{
			AddNode3dProperty(handle._obj, ref _box.[Friend]handle, _name);
		}

	}
}
