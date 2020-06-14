using System;

namespace ari
{
	[CRepr]
	struct PropertyReplicatorHandle
	{
		public uint32 Handle;
		public uint32 Index;
		public void* _obj = null;
		public Entity Owner = null;

		[CLink]
		static extern bool IsValidPropertyReplicator(ref uint32 _handle);

		bool IsValid() mut
		{
			if (Handle == uint32.MaxValue || Index == uint32.MaxValue)
				return false;

			return IsValidPropertyReplicator(ref Handle);
		}
	}

	public class PropertyReplicator
	{
		PropertyReplicatorHandle handle;

		public this(PropertyReplicatorHandle _handle)
		{
			this.handle = _handle;
		}

		[CLink]
		static extern void AddNode3dProperty(void* _obj, ref Node3dHandle _node, char8* _propertyName);

		// This function can replicate Position, Rotation and Scale
		public void AddProperty(Node3D _box, char8* _name)
		{
			AddNode3dProperty(handle._obj, ref _box.[Friend]handle, _name);
		}
	}
}
