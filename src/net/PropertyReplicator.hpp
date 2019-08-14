#pragma once
#include "en/ComponentHandle.hpp"
#include "Meta.h"
#include "core/containers/Array.hpp"
#include "core/defines.hpp"
#include "en/World.hpp"

namespace ari::net
{
    class PropertyReplicator
    {
    public:

		ARI_COMPONENT(PropertyReplicator);

		struct PropertyIndex
		{
			int Index;
			en::ComponentHandle<void> Component;
			void* PropertyClone;

			~PropertyIndex()
			{
				core::Memory::Free(PropertyClone);
			}
		};

    	core::Array<PropertyIndex> Properties;

		template <typename T>
        void AddProperty(const en::ComponentHandle<T>& cmp, const char* property_name)
		{
			int property_index = -1;
			int c = 0;
			void* clone = nullptr;
			meta::doForAllMembers<T>(
				[&cmp, &property_name, &property_index, &c, &clone](auto& member)
				{
					if (c == -1)
						return;
					if (strcmp(property_name, member.getName()) != 0)
					{
						property_index = c;
						using MemberT = meta::get_member_type<decltype(member)>;
						clone = core::Memory::Alloc(sizeof(MemberT));
						if (member.canGetConstRef()) {
							*((MemberT*)clone) = member.get(*cmp.Component);
						}
						else if (member.hasGetter()) {
							*((MemberT*)clone) = member.getCopy(*cmp.Component);
						}
						c = -1;
						return;
					}
					c++;
				}
			);
			if (property_index >= 0)
			{
				// We found the property add it to the list
				en::ComponentHandle<void> cmpVoid = { cmp.Handle, cmp.Index, (void*)cmp.Component };
				Properties.Add({ property_index, cmpVoid, clone });
			}
		}
    };
    
} // namespace ari::net
