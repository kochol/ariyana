#include "Entity.hpp"
#include "World.hpp"
#include "ComponentManager.hpp"

namespace ari::en
{
	ARI_COMPONENT_IMP(Entity);

	Entity::~Entity()
	{
		for (auto it = mComponents.begin(); it != mComponents.end(); ++it)
		{
			for (auto it_cmp = it->value.begin(); it_cmp != it->value.end(); ++it_cmp)
			{
				if (it_cmp->IsBased)
					continue;
				ComponentManager::DisposeComponent(p_world, it->key, it_cmp->handle, it_cmp->index, it_cmp->cmp);
			}
		}
	}
} // namespace ari::en
