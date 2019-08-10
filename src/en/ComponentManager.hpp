#pragma once

namespace ari::en
{
	struct World;

	class ComponentManager
	{
		struct ComponentData
		{
			ComponentHandleBase(*createFn)(World* world) = nullptr;
			void* getAllMembers;
		};

	public:

		template<typename T>
		static bool RegisterComponent()
		{
			if (!m_mComponentsData)
			{
				m_mComponentsData = core::Memory::New<core::Map<uint32_t, ComponentData>>();
			}
			ComponentData data;
			data.createFn = T::CreateComponent;
			m_mComponentsData->Add(T::Id, data);
			return true;
		}

		static ComponentHandleBase CreateComponent(uint32_t Id, World* pWorld)
		{
			return (*m_mComponentsData)[Id].createFn(pWorld);
		}

	private:

		static core::Map<uint32_t, ComponentData>*	m_mComponentsData;

	};

} // namespace ari::en
