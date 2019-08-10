#pragma once
#include "net.hpp"
#include "core/HandleManager.hpp"

namespace ari::net
{
	class CreateEntityMessage : public yojimbo::Message {
	public:
		uint32_t EntityHandle = core::aInvalidHandle;

		template <typename Stream>
		bool Serialize(Stream& stream) {
			serialize_uint32(stream, EntityHandle);
			return true;
		}

		YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
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
	YOJIMBO_MESSAGE_FACTORY_START(GameMessageFactory, (int)GameMessageType::COUNT);
	YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::CREATE_ENTITY, CreateEntityMessage);
	YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::DESTROY_ENTITY, DestroyEntityMessage);
	YOJIMBO_MESSAGE_FACTORY_FINISH();

} // namespace ari::net
