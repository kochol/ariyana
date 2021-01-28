using System;
using System.Collections;
using ari.io;
using ari.core;

namespace ari.en
{
	public class World
	{
		List<AriSystem> BeefSystems = new List<AriSystem>() ~ DeleteContainerAndItems!(_);

		// Constructor
		public this()
		{
		}

		// Destructor
		public ~this()
		{
		}

		public void Update(float _elapsedTime)
		{
			for (var sys in BeefSystems)
				sys.[Friend]Update(this, _elapsedTime);
		}

		// Emit
		public void Emit(ari_event* _event, ref WindowHandle _handle)
		{
		}

		// Add system
		public void AddSystem(AriSystem pSystem)
		{
			BeefSystems.Add(pSystem);
			pSystem.[Friend]Configure(this);
		}

		public void AddEntity(Entity _entity)
		{
			_entity.[Friend]world = this;
		}

		public static EntityHandle CreateEntity()
		{
			var p = MemoryPool<Entity>.Pool;
			p.Setup(p.MinGrow);
			Entity e = new:p [Friend]Entity();
			return EntityHandle(HandleManager<Entity>.CreateHandle(e), e);
		}

		public static EntityHandle CreateEntity<T>() where T : Entity
		{
			var p = MemoryPool<Entity>.Pool;
			p.Setup(p.MinGrow);
			T e = new:p [Friend]T();
			return EntityHandle(HandleManager<Entity>.CreateHandle(e), e);
		}

	}
}
