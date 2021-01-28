using System;

namespace ari.en
{
	[AttributeUsage(.Class, .AlwaysIncludeTarget | .ReflectAttribute, ReflectUser = .All, AlwaysIncludeUser = .IncludeAllMethods | .AssumeInstantiated)]
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
			if (cmp_id == base_id)
				Compiler.EmitTypeBody(type, scope $"""
					public static readonly uint32 Id = {cmp_id};
					public static readonly uint32 BaseId = {base_id};
	
					public virtual uint32 GetId() {{ return {cmp_id}; }}
					public virtual uint32 GetBaseId() {{ return {base_id}; }}
					""");
			else
				Compiler.EmitTypeBody(type, scope $"""
					public static readonly new uint32 Id = {cmp_id};
					public static readonly new uint32 BaseId = {base_id};

					public override uint32 GetId() {{ return {cmp_id}; }}
					public override uint32 GetBaseId() {{ return {base_id}; }}
					""");
		}
	}
}
