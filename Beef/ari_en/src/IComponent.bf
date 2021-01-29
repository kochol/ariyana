using System;

namespace ari.en
{
	interface IComponent
	{
		uint32 GetId();
		uint32 GetBaseId();

		static void AddComponent(World _world, ref EntityHandle _entity, ref ComponentHandle<IComponent> _cmp);
		static ComponentHandle<IComponent> CreateComponent();
		static void DeleteComponent(ref ComponentHandle<IComponent> _cmp);
		static void DisposeComponent(World _world, ref ComponentHandle<IComponent> _cmp);
	}
}
