using System;

namespace ari.en
{
	[AttributeUsage(.Class)]
	public struct ComponentAttribute : Attribute, IComptimeTypeApply
	{
		uint32 cmp_id;
		uint32 base_id;
		String cmp_name;
		String base_name;

		public this(String _cmp_name)
		{
			cmp_id = base_id = Hash.HashStringFNV32(_cmp_name);
			cmp_name = _cmp_name;
			base_name = null;
		}

		public this(String _cmp_name, String _base_name)
		{
			cmp_id = Hash.HashStringFNV32(_cmp_name);
			base_id = Hash.HashStringFNV32(_base_name);
			cmp_name = _cmp_name;
			base_name = _base_name;
		}

		public uint32 Id
 		{
			 get => cmp_id;
		}

		public uint32 BaseId
		{
			get => base_id;
		}

		[Comptime]
		public void ApplyToType(Type type)
		{
			String new_text = scope String();
			String override_text = scope String("virtual");
			String basename_text = scope String();

			if (cmp_id == base_id)
			{
				if (type.IsSubtypeOf(typeof(Node)) && cmp_name != "Node")
				{
					new_text = "new";
					override_text = "override";
				}
			}
			else
			{
				new_text = "new";
				override_text = "override";
				basename_text.AppendF(", {}", base_name);
			}

			Compiler.EmitTypeBody(type, scope $"""
					public static readonly {new_text} uint32 Id = {cmp_id};
					public static readonly {new_text} uint32 BaseId = {base_id};

					public {override_text} uint32 GetId() {{ return {cmp_id}; }}
					public {override_text} uint32 GetBaseId() {{ return {base_id}; }}

					public static void AddComponent(World _world, ref EntityHandle _entity, ref ComponentHandle<{cmp_name}> _cmp)
					{{
						_world.AddComponent<{cmp_name}{basename_text}>(ref _entity, ref _cmp);
					}}
					public static {new_text} void AddComponentGeneral(World _world, ref EntityHandle _entity, ref ComponentHandle<IComponent> _cmp)
					{{
						var cmp = _cmp.CastTo<{cmp_name}>();
						_world.AddComponent<{cmp_name}{basename_text}>(ref _entity, ref cmp);
						_cmp.Owner = cmp.Owner;
					}}

					public static {new_text} ComponentHandle<{cmp_name}> CreateComponent()
					{{
						return World.CreateComponent<{cmp_name}{basename_text}>();
					}}
					public static {new_text} ComponentHandle<IComponent> CreateComponentGeneral()
					{{
						return World.CreateComponent<{cmp_name}{basename_text}>().CastTo<IComponent>();
					}}

					public static {new_text} void DeleteComponentGeneral(ref ComponentHandle<IComponent> _cmp)
					{{
						var cmp = _cmp.CastTo<{cmp_name}>();
						World.DeleteComponent<{cmp_name}{basename_text}>(ref cmp);
					}}

					public static {new_text} void DisposeComponentGeneral(World _world, ref ComponentHandle<IComponent> _cmp)
					{{
						var cmp = _cmp.CastTo<{cmp_name}>();
						_world.DisposeComponent<{cmp_name}>(ref cmp);
					}}

					static bool is_registered_with_cmp_mgr = ComponentManager.RegisterComponent<{cmp_name}>({cmp_id});

					""");
		}
	}
}
