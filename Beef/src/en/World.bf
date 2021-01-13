using System;
using System.Collections;
using ari.io;

namespace ari.en
{
	public class World
	{
		void* _obj = null;
		List<AriSystem> BeefSystems = new List<AriSystem>() ~ DeleteContainerAndItems!(_);

		// Constructor
		[CLink]
		static extern void* CreateWorld();

		public this()
		{
			_obj = CreateWorld();
		}

		// Destructor
		[CLink]
		static extern void DeleteWorld(void* _obj);

		public ~this()
		{
			DeleteWorld(_obj);
			_obj = null;
		}

		[CLink]
		static extern void UpdateWorld(void* _obj, float _elapsedTime);

		public void Update(float _elapsedTime)
		{
			for (var sys in BeefSystems)
				sys.[Friend]Update(this, _elapsedTime);

			UpdateWorld(_obj, _elapsedTime);
		}

		[CLink]
		static extern void EmitOnInput(void* _world, ari_event* _event, ref WindowHandle _handle);

		// Emit
		public void Emit(ari_event* _event, ref WindowHandle _handle)
		{
			EmitOnInput(_obj, _event, ref _handle);
		}

		// Add system
		[CLink]
		static extern void AddSystemToWorld(void* _world, void* _system);

		public void AddSystem(AriSystem pSystem)
		{
			if (pSystem.[Friend]IsNativeSystem)
				AddSystemToWorld(_obj, pSystem.[Friend]_obj);
			else
				BeefSystems.Add(pSystem);
			pSystem.[Friend]Configure(this);
		}

		[CLink]
		static extern void AddEntityToWorld(void* _world, ref EntityHandle _entity);

		public void AddEntity(Entity _entity)
		{
			AddEntityToWorld(_obj, ref _entity.Handle);
			_entity.[Friend]world = this;
		}

		[CLink]
		static extern EntityHandle CreateEntityWorld();

		[CLink]
		static extern bool* GetEntityMembers(ref EntityHandle _entity);

		public static Entity CreateEntity()
		{
			var e = new [Friend]Entity(CreateEntityWorld());
			e.Replicates = GetEntityMembers(ref e.Handle);
			return e;
		}

		public static T CreateEntity<T>() where T : Entity
		{
			var e = new [Friend]T(CreateEntityWorld());
			e.Replicates = GetEntityMembers(ref e.Handle);
			return e;
		}

	}
}
