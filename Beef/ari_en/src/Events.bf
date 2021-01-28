namespace ari.en
{
	// Called when a new entity is created.
	struct OnEntityCreated
	{
		public EntityHandle Entity;
	}

	// Called when an entity is about to be destroyed.
	struct OnEntityDestroyed
	{
		public EntityHandle Entity;
	}

	// Called when a component is assigned (not necessarily created).
	struct OnComponentAssigned<T>
	{
		public EntityHandle Entity;
		public T Component;
	}

	// Called when a component is removed
	struct OnComponentRemoved<T>
	{
		public EntityHandle Entity;
		public T Component;
	}
}
