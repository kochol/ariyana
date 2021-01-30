using System;
using System.Collections;
using ari.io;
using ari.core;
using System.Threading;

namespace ari.en
{
	public class World
	{
		List<AriSystem> BeefSystems = new List<AriSystem>() ~ DeleteContainerAndItems!(_);

		Dictionary<uint32, // component id
			Dictionary<uint32, Entity>> // component handle
			components = new Dictionary<uint32, Dictionary<uint32, Entity>>() ~ DeleteDictionaryAndValues!(_);

		int turn_index = 0;

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

			turn_index++;
			if (turn_index > 2)
				turn_index = 0;

			// delete components
			for (int i = turn_index; i < dispose_lists.Count; i += 3)
			{
				for (var c in dispose_lists[i])
				{
					ComponentManager.DeleteComponent(ref c);
				}
				dispose_lists[i].Clear();
			}
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
		public void AddEntity(ref EntityHandle _entity)
		{
			_entity.Entity.[Friend]world = this;
			Emit<OnEntityCreated>(ref OnEntityCreated { Entity = _entity });
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

		public void AddComponent<T>(ref EntityHandle _entity, ref ComponentHandle<T> _cmp) where T : IComponent
		{
			let cmpId = _cmp.Component.GetId();

			// Add component to the world
			if (!components.ContainsKey(cmpId))
				components.Add(cmpId, new Dictionary<uint32, Entity>());
			components[cmpId].Add(_cmp.Handle, _entity.Entity);

			// Add component to the entity
			var en_cmps = _entity.Entity.[Friend]components;
			if (!en_cmps.ContainsKey(cmpId))
				en_cmps.Add(cmpId, new List<uint32>());
			en_cmps[cmpId].Add(_cmp.Handle);

			// Set the component owner
			_cmp.Owner = _entity.Entity;

			// Emit to the world
			Emit<OnComponentAssigned<T>>(ref OnComponentAssigned<T> { Entity = _entity, Component = _cmp.Component });
		}

		public void AddDerivedComponent<T, Base>(ref EntityHandle _entity, ref ComponentHandle<T> _cmp) where T : IComponent
			where Base : IComponent
		{
			let cmpId = _cmp.Component.GetBaseId();

			// Add component to the world
			if (!components.ContainsKey(cmpId))
				components.Add(cmpId, new Dictionary<uint32, Entity>());
			components[cmpId].Add(_cmp.Handle, _entity.Entity);

			// Add component to the entity
			var en_cmps = _entity.Entity.[Friend]components;
			if (!en_cmps.ContainsKey(cmpId))
				en_cmps.Add(cmpId, new List<uint32>());
			en_cmps[cmpId].Add(_cmp.Handle);

			// Emit to the world
			Emit<OnComponentAssigned<Base>>(ref OnComponentAssigned<Base> { Entity = _entity, Component = (Base)(IComponent)_cmp.Component });

			// Also add T class to the list
			AddComponent<T>(ref _entity, ref _cmp);
		}

		public static ComponentHandle<T> CreateComponent<T>() where T : IComponent, class, new
		{
			var p = ObjectPool<T>.Pool;
			p.Setup(p.MinGrow);
			T cmp = new:p [Friend]T();
			return ComponentHandle<T>(HandleManager<T>.CreateHandle(cmp), cmp, => HandleManager<T>.IsValid);
		}

		public static ComponentHandle<T> CreateComponent<T, Base>() where Base : IComponent
			where T : class, new, IComponent
		{
			var p = MemoryPool<Base>.Pool;
			p.Setup(p.MinGrow);
			T cmp = new:p [Friend]T();
			return ComponentHandle<T>(HandleManager<Base>.CreateHandle((Base)(IComponent)cmp), cmp, => HandleManager<T>.IsValid);
		}

		public static void DeleteComponent<T>(ref ComponentHandle<T> _cmp) where T : IComponent, delete
		{
			var p = ObjectPool<T>.Pool;
			delete:p _cmp.Component;
			HandleManager<T>.RemoveHandle(ref _cmp.Handle);
		}

		public static void DeleteComponent<T, Base>(ref ComponentHandle<T> _cmp) where T : IComponent, delete
		{
			var p = MemoryPool<Base>.Pool;
			delete:p _cmp.Component;
			HandleManager<Base>.RemoveHandle(ref _cmp.Handle);
		}

		[ThreadStatic]
		static List<List<ComponentHandle<IComponent>>> cmp_to_dispose = null;
		List<List<ComponentHandle<IComponent>>> dispose_lists = new List<List<ComponentHandle<IComponent>>>() ~ DeleteContainerAndItems!(_);
		Monitor dispose_lock = new Monitor() ~ delete _;

		public void DisposeComponent<T>(ref ComponentHandle<T> _cmp) where T : IComponent
		{
			if (cmp_to_dispose == null)
			{
				cmp_to_dispose = new List<List<ComponentHandle<IComponent>>>(3);
				dispose_lock.Enter();
				for (int i = 0; i < 3; i++)
				{
					cmp_to_dispose.Add(new List<ComponentHandle<IComponent>>());
					dispose_lists.Add(cmp_to_dispose[i]);
				}
				dispose_lock.Exit();
			}
			let id = _cmp.Component.GetId();
			let base_id = _cmp.Component.GetBaseId();
			cmp_to_dispose[turn_index].Add(_cmp.CastTo<IComponent>());
			components[id][_cmp.Handle] = null;

			_cmp.Owner = null;
			_cmp.Handle = uint32.MaxValue;

			if (id != base_id)
			{
				components[base_id][_cmp.Handle] = null;
			}
		}
	}
}
