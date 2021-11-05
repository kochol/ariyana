using ari.core;

namespace ari.en
{
	struct ComponentHandle<T> where T : IComponent, class
	{
		public uint32 Handle = uint32.MaxValue;
		public T Component = null;
		public Entity Owner = null;
		function bool(ref uint32) isValid = null;

		public this() {}

		public this(uint32 handle, T component, function bool(ref uint32) isValidFn) {
			Handle = handle;
			Component = component;
			isValid = isValidFn;
		}

		public ComponentHandle<TO> CastTo<TO>() where TO: IComponent, class
		{
			return ComponentHandle<TO>(Handle, (TO)(IComponent)Component, isValid);
		}

		public bool IsValid() mut
		{
			if (Handle == uint32.MaxValue)
				return false;

			return isValid(ref Handle);
		}
	}
}
