namespace ari
{
  	typealias TypeIndex = uint32;

    class TypeRegistry
    {
        public this()
        {
            index = nextIndex++;
        }

        public TypeIndex getIndex()
        {
            return index;
        }

        static TypeIndex nextIndex = 0;
        TypeIndex index;
    }

	class AriTypeId<T>
	{
		static TypeRegistry m_sTypeReg = new TypeRegistry() ~ delete _;

		public static TypeIndex GetTypeId()
		{
			return m_sTypeReg.getIndex();
		}

	}
}
