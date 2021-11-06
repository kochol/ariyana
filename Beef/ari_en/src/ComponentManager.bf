using System.Collections;

namespace ari.en
{
	class ComponentManager
	{
		struct ComponentData
		{
			public function void(World, ref EntityHandle, ref ComponentHandle<IComponent>) AddFn;
			public function ComponentHandle<IComponent>() CreateFn;
			public function void(ref ComponentHandle<IComponent>) DeleteFn;
			public function void(World, ref ComponentHandle<IComponent>) DisposeFn;
		}

		static Dictionary<uint32, ComponentData> components_data = new Dictionary<uint32, ComponentData>() ~ delete _;

		public static bool RegisterComponent<T>(uint32 id) where T: IComponent
		{
			ComponentData data;
			data.AddFn = => T.AddComponentGeneral;
			data.CreateFn = => T.CreateComponentGeneral;
			data.DeleteFn = => T.DeleteComponentGeneral;
			data.DisposeFn = => T.DisposeComponentGeneral;

			components_data.Add(id, data);

			return true;
		}

		public static void AddComponent(World _world, ref EntityHandle _entity, ref ComponentHandle<IComponent> _cmp)
		{
			components_data[_cmp.Component.GetId()].AddFn(_world, ref _entity, ref _cmp);
		}

		public static ComponentHandle<IComponent> CreateComponent(uint32 cmp_id)
		{
			return components_data[cmp_id].CreateFn();
		}

		public static void DeleteComponent(ref ComponentHandle<IComponent> _cmp)
		{
			components_data[_cmp.Component.GetId()].DeleteFn(ref _cmp);
		}

		public static void DisposeComponent(World _world, ref ComponentHandle<IComponent> _cmp)
		{
			components_data[_cmp.Component.GetId()].DisposeFn(_world, ref _cmp);
		}
	}
}
