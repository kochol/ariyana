using System;
using ari.en;

namespace ari.net
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
	}
}
