using System;

namespace ari.core
{
	public interface IResourceLoader
	{
		public bool CanLoadResource(String path);

		public T LoadResource<T>(String path);
	}
}
