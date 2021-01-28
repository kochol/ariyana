using System;
using ari.core;

namespace ari.en
{
	public struct EntityHandle
	{
		public uint32 Handle = uint32.MaxValue;
		public Entity Entity = null;

		public this() { }

		public this(uint32 handle, Entity entity)
		{
			Handle = handle;
			Entity = entity;
		}

		public bool IsValid() mut
		{
			if (Handle == uint32.MaxValue)
				return false;

			return HandleManager<Entity>.IsValid(ref Handle);
		}
	}

	[Component("Entity", "Node")]
	public class Entity: Node
	{
		public bool Replicates;
		protected World world = null;

		protected this()
		{
		}

		public ~this()
		{
		}
	}
}
