#pragma once
#include "yojimbo.h"
#include "core/log.h"

namespace ari::en
{
	struct World;

	enum class StreamType
	{
		ReadStream,
		WriteStream,
		MeasureStream
	};

	class ComponentManager
	{
		struct ComponentData
		{
			ComponentHandle<void>(*createFn)(World*) = nullptr;
			void(*AddComponentFn)(ari::en::World* _world,
				const ari::en::EntityHandle& _entity, ari::en::ComponentHandle<void> cmp) = nullptr;
			bool(*serializeFn)(yojimbo::WriteStream&, void*) = nullptr;
			bool(*deserializeFn)(yojimbo::ReadStream&, void*) = nullptr;
			bool(*serializeMeasureFn)(yojimbo::MeasureStream&, void*) = nullptr;
		};

	public:

		template<typename T>
		static bool RegisterComponent(char* cmpName)
		{
			log_debug("Component %s Id is %u and its base id is %u", cmpName, T::Id, T::GetBaseId());
			if (!m_mComponentsData)
			{
				m_mComponentsData = core::Memory::New<core::Map<uint32_t, ComponentData>>();
			}
			ComponentData data;
			data.createFn = T::CreateComponent;
			data.AddComponentFn = T::AddComponent;
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

		static void AddComponent(uint32_t Id, ari::en::World* _world,
			const ari::en::EntityHandle& _entity, ari::en::ComponentHandle<void> cmp)
		{
			(*m_mComponentsData)[Id].AddComponentFn(_world, _entity, cmp);
		}

		static bool Serialize(uint32_t Id, void* stream, void* obj)
		{
			return (*m_mComponentsData)[Id].serializeFn(*((yojimbo::WriteStream*)stream), obj);
		}
		static bool Deserialize(uint32_t Id, void* stream, void* obj)
		{
			return (*m_mComponentsData)[Id].deserializeFn(*((yojimbo::ReadStream*)stream), obj);
		}
		static bool SerializeMeasure(uint32_t Id, void* stream, void* obj)
		{
			return (*m_mComponentsData)[Id].serializeMeasureFn(*((yojimbo::MeasureStream*)stream), obj);
		}

		template<typename T>
		static bool S1(uint32_t Id, T& stream, void* obj)
		{
			return Deserialize(Id, stream, obj);
		}

		template <typename T>
		static bool Serialize(StreamType type, uint32_t Id, T& stream, void* obj)
		{
			if (type == StreamType::ReadStream)
				return S1(Id, stream, obj);
			/*if (type == StreamType::WriteStream)
				return Serialize(Id, stream, obj);
			if (type == StreamType::MeasureStream)
				return SerializeMeasure(Id, stream, obj);*/
			return true;
		}

	private:

		static core::Map<uint32_t, ComponentData>*	m_mComponentsData;


	};

} // namespace ari::en
