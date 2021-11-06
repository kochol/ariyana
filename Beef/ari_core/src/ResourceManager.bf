using System;
using System.Collections;

namespace ari.core
{
	public static class ResourceManager<T> where T : Resource
	{
		// Resources
		static Dictionary<String, T> _resources = new Dictionary<String, T>() ~ delete _;
		// Resource loaders
		static List<IResourceLoader> _loaders = new List<IResourceLoader>() ~ delete _;

		public static T LoadResource(String path)
		{
			T r;
			if (_resources.TryGetValue(path, out r))
				return r;

			// Load the resource
			for (var l in _loaders)
			{
				if (l.CanLoadResource(path))
				{
					r = l.LoadResource<T>(path);
					_resources.Add(path, r);
					return r;
				}
			}

			return null;
		}

		public static void AddResourceLoader(IResourceLoader loader)
		{
			_loaders.Add(loader);
		}
	}
}
