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

        static TypeIndex nextIndex;
        TypeIndex index;
    }

	class AriTypeId<T>
	{
		static TypeRegistry m_sTypeReg;

		public static TypeIndex GetTypeId()
		{
			return m_sTypeReg.getIndex();
		}

	}
}
