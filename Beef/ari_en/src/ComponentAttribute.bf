using System;

namespace ari.en
{
	[AttributeUsage(.Struct | .Class, .AlwaysIncludeTarget | .ReflectAttribute, ReflectUser = .All, AlwaysIncludeUser = .IncludeAllMethods | .AssumeInstantiated)]
	public struct ComponentAttribute : Attribute, IComptimeTypeApply
	{
		uint32 cmp_id;
		uint32 base_id;

		public this(String cmp_name)
		{
			cmp_id = base_id = Hash.HashStringFNV32(cmp_name);
		}

		public this(String cmp_name, String base_name)
		{
			cmp_id = Hash.HashStringFNV32(cmp_name);
			base_id = Hash.HashStringFNV32(base_name);
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
			Compiler.EmitTypeBody(type, scope $"""
				public static uint32 Id = {cmp_id};
				public static uint32 BaseId = {base_id};

				public uint32 GetId() {{ return {cmp_id}; }}
				public uint32 GetBaseId() {{ return {base_id}; }}
				""");
		}
	}
}
