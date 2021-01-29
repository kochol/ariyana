using System;

namespace ari.en
{
	interface IComponent
	{
		uint32 GetId();
		uint32 GetBaseId();

		static void AddComponentGeneral(World _world, ref EntityHandle _entity, ref ComponentHandle<IComponent> _cmp);
		static ComponentHandle<IComponent> CreateComponentGeneral();
		static void DeleteComponentGeneral(ref ComponentHandle<IComponent> _cmp);
		static void DisposeComponentGeneral(World _world, ref ComponentHandle<IComponent> _cmp);
	}
}
