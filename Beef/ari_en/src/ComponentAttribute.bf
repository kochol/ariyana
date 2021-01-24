using System;

namespace ari.en
{
	[AttributeUsage(.Struct | .Class, .AlwaysIncludeTarget | .ReflectAttribute, ReflectUser = .All, AlwaysIncludeUser = .IncludeAllMethods | .AssumeInstantiated)]
	public struct ComponentAttribute : Attribute
	{
		uint32 cmp_id;
		uint32 base_id;

		public this(Type _cmp_type)
		{
			var str = scope String();
			_cmp_type.GetName(str);
			cmp_id = base_id = Hash.HashStringFNV32(str);
		}

		public this(Type _cmp_type, Type _base_type)
		{
			var str = scope String();
			_cmp_type.GetName(str);
			cmp_id = Hash.HashStringFNV32(str);
			str.Clear();
			_base_type.GetName(str);
			base_id = Hash.HashStringFNV32(str);
		}

		public uint32 Id
 		{
			 get => cmp_id;
		}

		public uint32 BaseId
		{
			get => base_id;
		}
	}
}
