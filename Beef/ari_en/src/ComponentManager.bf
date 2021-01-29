using System.Collections;

namespace ari.en
{
	class ComponentManager
	{
		struct ComponentData
		{
			public function ComponentHandle<IComponent>() CreateFn;
			public function void(World, ref ComponentHandle<IComponent>) DisposeFn;
			public function void(ref ComponentHandle<IComponent>) DeleteFn;
			public function void(World, ref EntityHandle, ref ComponentHandle<IComponent>) AddFn;
		}

		static Dictionary<uint32, ComponentData> components_data = new Dictionary<uint32, ComponentData>() ~ delete _;

		public static bool RegisterComponent<T>(uint32 id) where T: IComponent
		{
			ComponentData data;
			//data.CreateFn = => World.CreateComponent<T>;
			return true;
		}
	}
}
