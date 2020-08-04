#pragma once
#include "net.hpp"
#include "core/HandleManager.hpp"
#include "en/World.hpp"
#include "en/ComponentManager.hpp"
#include "core/defines.hpp"
#include "core/log.h"
#include "PropertyReplicator.hpp"
#include "RPC.hpp"
#include "NetworkSystem.hpp"

namespace ari::net
{
	class ClientSystem;
	class CreateEntityMessage : public yojimbo::Message {
	public:
		en::EntityHandle Entity;
		en::World* World = nullptr;
		ClientSystem* client_system = nullptr;

		template <typename Stream>
		bool Serialize(Stream& stream, bool Measure = false) 
		{
			a_assert(World);
			serialize_uint32(stream, Entity.Handle);
			if (Stream::IsWriting)
			{
				Entity->GetComponents([&stream, &Measure](uint32_t& CmpId, uint32_t& CmpHandle, void* Cmp)
					{
						if (CmpId == PropertyReplicator::Id)
							return true; // Don't send the PropertyReplicator component

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
		uint32_t CmpHandle;
		uint32_t CmpId;
		int MemberIndex;
		void* Component = nullptr;

		template <typename Stream>
		bool Serialize(Stream& stream, bool Measure = false) 
		{
			serialize_uint32(stream, CmpId);
			serialize_uint32(stream, CmpHandle);
			serialize_int(stream, MemberIndex, 0, 31);
			if (Stream::IsWriting)
			{
				a_assert(Component);
				if (Measure)
				{
					if (!en::ComponentManager::SerializeMeasure(CmpId, (void*)& stream, Component, MemberIndex))
						return false;
				}
				else
				{
					if (!en::ComponentManager::Serialize(CmpId, (void*)& stream, Component, MemberIndex))
						return false;
				}
			}
			else
			{
				a_assert(client_system);
				Component = GetComponent(CmpId, CmpHandle);
				if (!en::ComponentManager::Deserialize(CmpId, (void*)& stream, Component, MemberIndex))
					return false;
			}
			return true;
		}

		bool SerializeInternal(class yojimbo::ReadStream& stream) override { return Serialize(stream); };
		bool SerializeInternal(class yojimbo::WriteStream& stream) override { return Serialize(stream); };
		bool SerializeInternal(class yojimbo::MeasureStream& stream) override { return Serialize(stream, true); };

		void* GetComponent(const uint32_t& component_id, const uint32_t& component_server_handle);

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

	class RpcCallMessage: public yojimbo::Message
	{
	public:

		RPC* rpc = nullptr;

		template <typename Stream>
		bool Serialize(Stream& stream, bool Measure = false) 
		{
			if (Stream::IsWriting)
			{
				a_assert(rpc);
				serialize_uint32(stream, rpc->function_hash);
				if (!Measure)
				{
					if (!rpc->Serialize((void*)& stream))
						return false;
				}
				else
				{
					if (!rpc->SerializeMeasure((void*)& stream))
						return false;
				}
			}
			else
			{
				uint32_t function_hash;
				serialize_uint32(stream, function_hash);

				// Get RPC
				rpc = GetRPC(function_hash);
				if (!rpc->Deserialize((void*)& stream))
					return false;
			}
			return true;
		}

		bool SerializeInternal(class yojimbo::ReadStream& stream) override { return Serialize(stream); };
		bool SerializeInternal(class yojimbo::WriteStream& stream) override { return Serialize(stream); };
		bool SerializeInternal(class yojimbo::MeasureStream& stream) override { return Serialize(stream, true); };

	};

	class CRpcCallMessage : public yojimbo::Message
	{
	public:

		~CRpcCallMessage()
		{
			g_on_delete_rpc(rpc_index);
		}

		void AddRef() const
		{
			g_on_add_ref_rpc(rpc_index);
		}

		void* rpc = nullptr;
		uint32_t rpc_index = 0;

		template <typename Stream>
		bool Serialize(Stream& stream, bool Measure = false) 
		{
			if (Stream::IsWriting)
			{
				a_assert(rpc);
				if (!Measure)
				{
					return g_on_serialize((void*)&stream, rpc, rpc_index);
				}
				else
				{
					return g_on_serialize_measure((void*)&stream, rpc, rpc_index);
				}
			}
			else
			{
				return g_on_deserialize((void*)&stream, &rpc, &rpc_index);
			}
		}

		bool SerializeInternal(class yojimbo::ReadStream& stream) override { return Serialize(stream); };
		bool SerializeInternal(class yojimbo::WriteStream& stream) override { return Serialize(stream); };
		bool SerializeInternal(class yojimbo::MeasureStream& stream) override { return Serialize(stream, true); };

	};

	// the message factory
	ARI_MESSAGE_FACTORY_START(GameMessageFactory, (int)GameMessageType::COUNT)
	YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::DESTROY_ENTITY, DestroyEntityMessage)
	YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::RPC_CALL, RpcCallMessage)
	YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::CRPC_CALL, CRpcCallMessage)
	YOJIMBO_MESSAGE_FACTORY_FINISH()

} // namespace ari::net
