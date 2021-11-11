namespace ari.core
{
	public enum ResourceState
	{
		NotLoaded,
		Loading,
		Failed,
		NotFound,
		Ready,
		Deleted,
	}

	public abstract class Resource
	{
		protected ResourceState state = ResourceState.NotLoaded;

		public ResourceState State => state;
	}
}
