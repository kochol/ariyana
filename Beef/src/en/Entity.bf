using System;

namespace ari
{
	[CRepr]
	public struct EntityHandle
	{
		public uint32 Handle = uint32.MaxValue;
		public uint32 Index = uint32.MaxValue;
		public void* _entity = null;

		[CLink]
		static extern bool IsValidEntity(ref uint32 _entityHandle);

		public bool IsValid() mut
		{
			if (Handle == uint32.MaxValue || Index == uint32.MaxValue)
				return false;

			return IsValidEntity(ref Handle);
		}
	}

	public class Entity: Node
	{
		public bool* Replicates;
		public EntityHandle Handle;
		protected World world = null;

		protected this(EntityHandle _handle): base(_handle._entity)
		{
			Handle = _handle;
		}

		[CLink]
		static extern void DeleteEntityWorld(ref EntityHandle _entity);

		public ~this()
		{
			DeleteEntityWorld(ref Handle);
		}
	}
}
