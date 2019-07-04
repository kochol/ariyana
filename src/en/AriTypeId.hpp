#pragma once
#include <cstdint>

namespace ari
{
   	typedef uint32_t TypeIndex;

    class TypeRegistry
    {
    public:
        TypeRegistry()
        {
            index = nextIndex;
            ++nextIndex;
        }

        TypeIndex getIndex() const
        {
            return index;
        }

    private:
        static TypeIndex nextIndex;
        TypeIndex index;
    };

	template<typename T>
	class AriTypeId
	{
		static TypeRegistry m_sTypeReg;

	public:

		static TypeIndex GetTypeId()
		{
			return m_sTypeReg.getIndex();
		}

	};

	template<typename T>
	TypeRegistry AriTypeId<T>::m_sTypeReg;

} // namespace ari
