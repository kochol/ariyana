#pragma once
#include "net.hpp"
#include "core/HandleManager.hpp"
#include "en/World.hpp"
#include "en/ComponentManager.hpp"
#include "core/defines.hpp"
#include "core/log.h"

namespace ari::net
{
	class CreateEntityMessage : public yojimbo::Message {
	public:
		en::EntityHandle Entity;
		en::World* World = nullptr;

		CreateEntityMessage()
		{
			log_debug("CreateEntityMessage");
		}

		template <typename Stream>
		bool Serialize(Stream& stream, bool Measure = false) {
			a_assert(World);
			serialize_uint32(stream, Entity.Handle);
			if (Stream::IsWriting)
			{
				World->GetEntityComponents(Entity, [&stream, &Measure](uint32_t& CmpId, const uint32_t& CmpHandle, void* Cmp)
					{
						serialize_uint32(stream, CmpId);
						if (Measure)
						{
							if (!en::ComponentManager::SerializeMeasure(CmpId, (void*)&stream, Cmp))
								return false;
						}
						else
						{
							if (!en::ComponentManager::Serialize(CmpId, (void*)&stream, Cmp))
								return false;
						}
						return true;
					});
				uint32_t end_of_write = 0;
				serialize_uint32(stream, end_of_write);
			}
			else
			{
				// Create the entity
				Entity = World->CreateEntity();

				// Now create the components
				do
				{
					uint32_t CmpId = 0;
					serialize_uint32(stream, CmpId);
					if (CmpId == 0) // Check for the end of components
						return true;

					auto cmp = en::ComponentManager::CreateComponent(CmpId, World);
					if (!en::ComponentManager::Deserialize(CmpId, (void*)&stream, cmp.Component))
						return false;

					en::ComponentManager::AddComponent(CmpId, World, Entity, cmp);
				}
				while (true);
			}
			return true;
		}

		bool SerializeInternal(class yojimbo::ReadStream& stream) override { return Serialize(stream); };          
		bool SerializeInternal(class yojimbo::WriteStream& stream) override { return Serialize(stream); };          
		bool SerializeInternal(class yojimbo::MeasureStream& stream) override { return Serialize(stream, true); };

	};

	class DestroyEntityMessage : public yojimbo::Message {
	public:
		uint32_t EntityHandle = core::aInvalidHandle;

		template <typename Stream>
		bool Serialize(Stream& stream) {
			serialize_uint32(stream, EntityHandle);
			return true;
		}

		YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
	};

	// the message factory
	ARI_MESSAGE_FACTORY_START(GameMessageFactory, (int)GameMessageType::COUNT)
	YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::DESTROY_ENTITY, DestroyEntityMessage)
	YOJIMBO_MESSAGE_FACTORY_FINISH()

} // namespace ari::net
