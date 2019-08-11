#pragma once
#include "yojimbo.h"

namespace ari::en
{
	struct World;

	class ComponentManager
	{
		struct ComponentData
		{
			ComponentHandle<void>(*createFn)(World*) = nullptr;
			bool(*serializeFn)(yojimbo::WriteStream&, void*) = nullptr;
			bool(*deserializeFn)(yojimbo::ReadStream&, void*) = nullptr;
			bool(*serializeMeasureFn)(yojimbo::MeasureStream&, void*) = nullptr;
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
			data.serializeFn = &T::Serialize;
			data.deserializeFn = &T::Serialize;
			data.serializeMeasureFn = &T::Serialize;
			m_mComponentsData->Add(T::Id, data);
			return true;
		}

		static ComponentHandle<void> CreateComponent(uint32_t Id, World* pWorld)
		{
			return (*m_mComponentsData)[Id].createFn(pWorld);
		}

		static bool Serialize(uint32_t Id, yojimbo::WriteStream& stream, void* obj)
		{
			return (*m_mComponentsData)[Id].serializeFn(stream, obj);
		}
		static bool Deserialize(uint32_t Id, yojimbo::ReadStream& stream, void* obj)
		{
			return (*m_mComponentsData)[Id].deserializeFn(stream, obj);
		}
		static bool SerializeMeasure(uint32_t Id, yojimbo::MeasureStream& stream, void* obj)
		{
			return (*m_mComponentsData)[Id].serializeMeasureFn(stream, obj);
		}

	private:

		static core::Map<uint32_t, ComponentData>*	m_mComponentsData;

	};

} // namespace ari::en
