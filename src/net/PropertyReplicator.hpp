#pragma once
#include "en/ComponentHandle.hpp"
#include "Meta.h"
#include "core/containers/Array.hpp"
#include "core/defines.hpp"
#include "en/World.hpp"
#include "core/log.h"

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
			uint32_t ComponentId;
			bool(*isDiffFn)(void*, void*, int);

			PropertyIndex() = default;

			PropertyIndex(PropertyIndex& other)
			{
				Index = other.Index;
				Component = other.Component;
				PropertyClone = other.PropertyClone;
				ComponentId = other.ComponentId;
				isDiffFn = other.isDiffFn;
				other.PropertyClone = nullptr;
			}

			PropertyIndex(PropertyIndex&& other)
			{
				Index = other.Index;
				Component = other.Component;
				PropertyClone = other.PropertyClone;
				ComponentId = other.ComponentId;
				isDiffFn = other.isDiffFn;
				other.PropertyClone = nullptr;
			}

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
					if (strcmp(property_name, member.getName()) == 0)
					{
						property_index = c;
						using MemberT = meta::get_member_type<decltype(member)>;
						clone = core::Memory::Alloc(sizeof(MemberT));
						if (member.canGetConstRef()) {
							*((MemberT*)clone) = member.get(*cmp.Component);
							MemberT* temp = (MemberT*)clone;
							int hj = 0;
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
				PropertyIndex property;
				property.Component = cmpVoid;
				property.ComponentId = cmp.Component->GetId();
				property.Index = property_index;
				property.PropertyClone = clone;
				property.isDiffFn = T::IsDiff;
				Properties.Add(property);
			}
		}
    };
    
} // namespace ari::net
