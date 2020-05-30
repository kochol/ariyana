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
			void(*DisposeFn)(ari::en::World* _world, const ari::en::ComponentHandle<void>& cmp) = nullptr;
			void(*DeleteFn)(const ari::en::ComponentHandle<void>& cmp) = nullptr;
			void(*AddComponentFn)(ari::en::World* _world,
				const ari::en::EntityHandle& _entity, ari::en::ComponentHandle<void> cmp) = nullptr;
			bool(*serializeFn)(yojimbo::WriteStream&, void*, const int&) = nullptr;
			bool(*deserializeFn)(yojimbo::ReadStream&, void*, const int&) = nullptr;
			bool(*serializeMeasureFn)(yojimbo::MeasureStream&, void*, const int&) = nullptr;
		};

	public:

		template<typename T>
		static bool RegisterComponent(char* cmpName)
		{
			log_debug("Component %s Id is %u", cmpName, T::Id);
			if (!m_mComponentsData)
			{
				m_mComponentsData = core::Memory::New<core::Map<uint32_t, ComponentData>>();
			}
			ComponentData data;
			data.createFn = T::CreateComponent;
			data.DisposeFn = T::DisposeComponent;
			data.DeleteFn = T::DeleteComponent;
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

		static void DisposeComponent(ari::en::World* _world, const uint32_t& Id, const uint32_t& handle, const uint32_t& index, void* cmp)
		{
			(*m_mComponentsData)[Id].DisposeFn(_world, { handle, index, cmp, nullptr });
		}

		static void DeleteComponent(const uint32_t& Id, const uint32_t& handle, const uint32_t& index)
		{
			(*m_mComponentsData)[Id].DeleteFn({ handle, index, nullptr, nullptr });
		}

		static void AddComponent(uint32_t Id, ari::en::World* _world,
			const ari::en::EntityHandle& _entity, ari::en::ComponentHandle<void> cmp)
		{
			(*m_mComponentsData)[Id].AddComponentFn(_world, _entity, cmp);
		}

		static bool Serialize(uint32_t Id, void* stream, void* obj, const int& member_index = -1)
		{
			return (*m_mComponentsData)[Id].serializeFn(*((yojimbo::WriteStream*)stream), obj, member_index);
		}

		static bool Deserialize(uint32_t Id, void* stream, void* obj, const int& member_index = -1)
		{
			return (*m_mComponentsData)[Id].deserializeFn(*((yojimbo::ReadStream*)stream), obj, member_index);
		}

		static bool SerializeMeasure(uint32_t Id, void* stream, void* obj, const int& member_index = -1)
		{
			return (*m_mComponentsData)[Id].serializeMeasureFn(*((yojimbo::MeasureStream*)stream), obj, member_index);
		}

		template <typename T>
		static bool IsDiff(void* clone, void* obj, int index)
		{
			int c = 0;
			bool is_same = false;
			meta::doForAllMembers<T>(
				[&obj, &index, &c, &clone, &is_same](auto& member)
				{
					if (c == -1)
						return;
					if (c == index)
					{
						c = -1;
						using MemberT = meta::get_member_type<decltype(member)>;
						if (member.canGetConstRef()) {
							is_same = *((MemberT*)clone) == member.get(*((T*)obj));
							if (!is_same)
								* ((MemberT*)clone) = member.get(*((T*)obj));
						}
						else if (member.hasGetter()) {
							is_same = *((MemberT*)clone) == member.getCopy(*((T*)obj));
							if (!is_same)
								* ((MemberT*)clone) = member.getCopy(*((T*)obj));
						}
						return;
					}
					c++;
				}
			);

			return !is_same;
		}

	private:

		static core::Map<uint32_t, ComponentData>*	m_mComponentsData;


	};

} // namespace ari::en
