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

		///////////////////////////////////////////////////////
		// Events
		//////////////////////////////////////////////////////
		Dictionary<TypeIndex, List<Object>> subscribers = new System.Collections.Dictionary<uint32, System.Collections.List<System.Object>>()
			~ DeleteDictionaryAndValues!(_);

		// Emit
		public void Emit<T>(ref T event)
		{
			let index = AriTypeId<T>.GetTypeId();
			if (subscribers.ContainsKey(index))
			{
				for (var s in subscribers[index])
				{
					var d = (delegate void(ref T))s;
					d.Invoke(ref event);
				}
			}
		}

		// Subscribe
		public void Subscribe<T>(delegate void(ref T) fn)
		{
			let index = AriTypeId<T>.GetTypeId();
			if (!subscribers.ContainsKey(index))
			{
				subscribers.Add(index, new List<Object>());
			}
			subscribers[index].Add(fn);
		}

		// Unsubscribe
		public void Unsubscribe<T>(delegate void(ref T) fn)
		{
			let index = AriTypeId<T>.GetTypeId();
			subscribers[index].Remove(fn);
		}

		//////////////////////////////////////////////////
		// Systems
		/////////////////////////////////////////////////

		// Add system
		public void AddSystem(AriSystem pSystem)
		{
			BeefSystems.Add(pSystem);
			pSystem.[Friend]Configure(this);
		}

		//////////////////////////////////////////////
		// Entity
		/////////////////////////////////////////////
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

		public static void DeleteEntity(ref EntityHandle entity)
		{
			HandleManager<Entity>.RemoveHandle(ref entity.Handle);
			var p = MemoryPool<Entity>.Pool;
			delete:p entity.Entity;
		}

		/////////////////////////////////////////////////
		// Components
		////////////////////////////////////////////////

		static ComponentHandle<T> CreateComponent<T>() where T : IComponent, class, new
		{
			var p = ObjectPool<T>.Pool;
			p.Setup(p.MinGrow);
			T cmp = new:p [Friend]T();
			return ComponentHandle<T>(HandleManager<T>.CreateHandle(cmp), cmp, => HandleManager<T>.IsValid);
		}
	}
}
