using System;

namespace ari.en
{
	[AttributeUsage(.Struct | .Class, .AlwaysIncludeTarget | .ReflectAttribute, ReflectUser = .All, AlwaysIncludeUser = .IncludeAllMethods | .AssumeInstantiated)]
	public struct ComponentAttribute : Attribute
	{
		Type cmp_type;
		Type base_type;

		public this(Type _cmp_type)
		{
			cmp_type = _cmp_type;
			base_type = null;
		}

		public this(Type _cmp_type, Type _base_type)
		{
			cmp_type = _cmp_type;
			base_type = _base_type;
		}
	}
}
