#pragma once
#include "net.hpp"
#include "core/HandleManager.hpp"
#include "en/World.hpp"
#include "en/ComponentManager.hpp"
#include "core/defines.hpp"
#include "core/log.h"

namespace ari::net
{
	class ClientSystem;
	class CreateEntityMessage : public yojimbo::Message {
	public:
		en::EntityHandle Entity;
		en::World* World = nullptr;
		ClientSystem* client_system = nullptr;

		template <typename Stream>
		bool Serialize(Stream& stream, bool Measure = false) {
			a_assert(World);
			serialize_uint32(stream, Entity.Handle);
			if (Stream::IsWriting)
			{
				World->GetEntityComponents(Entity, [&stream, &Measure](uint32_t& CmpId, uint32_t& CmpHandle, void* Cmp)
					{
						serialize_uint32(stream, CmpId);
						serialize_uint32(stream, CmpHandle);
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
				a_assert(client_system);
				// Check if we already have this entity
				uint32_t server_entity_handle = Entity.Handle;
				if (GetEntity(server_entity_handle).Handle != core::aInvalidHandle)
					return true;

				// Create the entity
				Entity = World->CreateEntity();
				AddEntity(server_entity_handle, Entity.Handle);

				// Now create the components
				do
				{
					uint32_t CmpId = 0;
					serialize_uint32(stream, CmpId);
					if (CmpId == 0) // Check for the end of components
						return true;

					uint32_t CmpHandle = 0;
					serialize_uint32(stream, CmpHandle);

					auto cmp = en::ComponentManager::CreateComponent(CmpId, World);
					// Add component handle to client system
					AddComponent(CmpId, CmpHandle, cmp.Handle);

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

		en::EntityHandle GetEntity(const uint32_t& server_entity_handle);

		void AddEntity(const uint32_t& server_entity_handle,
			const uint32_t& client_entity_handle);

		void AddComponent(const uint32_t& component_id, const uint32_t& component_server_handle,
			const uint32_t& component_client_handle);
	};

	class UpdateEntityMessage : public yojimbo::Message
	{
	public:	
		ClientSystem* client_system = nullptr;

		template <typename Stream>
		bool Serialize(Stream& stream) {
			if (Stream::IsWriting)
			{
			}
			else
			{
				a_assert(client_system);
			}
			return true;
		}

		YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
	};

	class DestroyEntityMessage : public yojimbo::Message 
	{
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
