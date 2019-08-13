#include "yojimbo.h"

namespace ari::net
{
	template<typename Class,
		typename Stream,
		typename>
	bool Serialize(Stream & stream, Class & obj, const int& member_index)
	{
		int c = -1;
		meta::doForAllMembers<Class>(
			[&obj, &stream, &c, &member_index](auto& member)
			{
				c++;
				if (member_index != -1 && member_index != c)
					return;
				using MemberT = meta::get_member_type<decltype(member)>;
				if (Stream::IsWriting)
				{
					if (member.canGetConstRef()) {
						Serialize(stream, member.get(obj));
					}
					else if (member.hasGetter()) {
						Serialize(stream, member.getCopy(obj)); // passing copy as const ref, it's okay
					}
				}
				else
				{
					if (member.canGetRef()) {
						Serialize(stream, member.getRef(obj));
					}
					else if (member.hasSetter()) {
						assert(false); // Not implemented yet.
						//member.set(obj, objName.template get<MemberT>());
					}
					else {
						assert(false); // ("Error: can't deserialize member because it's read only");
					}
				}
			}
		);

		return true;
	}

	template <typename Class,
		typename Stream,
		typename,
		typename>
	bool Serialize(Stream & stream, Class & obj, const int& member_index)
	{
		return SerializeBasic<Class, Stream>(stream, obj);
	}

	template <typename Class, typename Stream>
	bool SerializeBasic(Stream& stream, Class& obj)
	{
		serialize_bytes(stream, (uint8_t*)&obj, sizeof(Class));
		return true;
	}

} // namespace ari::net
