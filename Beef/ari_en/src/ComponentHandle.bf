using ari.core;

namespace ari.en
{
	struct ComponentHandle<T> where T : IComponent, class
	{
		public uint32 Handle = uint32.MaxValue;
		public T Component = null;
		public Entity Owner = null;
		function bool(ref uint32) isValid;

		public bool IsValid() mut
		{
			if (Handle == uint32.MaxValue)
				return false;

			return isValid(ref Handle);
		}
	}
}
