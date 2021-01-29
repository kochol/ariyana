using System;
using ari.core;
using System.Collections;

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

		protected Dictionary<uint32, // component id
			List<uint32>> // component handle
			components = new Dictionary<uint32, List<uint32>>() ~ DeleteDictionaryAndValues!(_);

		protected this()
		{
		}

		public ~this()
		{
		}
	}
}
