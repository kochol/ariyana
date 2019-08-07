#pragma once
#include "net.hpp"

namespace ari::net
{
	class TestMessage : public yojimbo::Message {
	public:
		int m_data;

		TestMessage() :
			m_data(0) {}

		template <typename Stream>
		bool Serialize(Stream& stream) {
			serialize_int(stream, m_data, 0, 512);
			return true;
		}

		YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
	};

	// the message factory
	YOJIMBO_MESSAGE_FACTORY_START(GameMessageFactory, (int)GameMessageType::COUNT);
	YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::TEST, TestMessage);
	YOJIMBO_MESSAGE_FACTORY_FINISH();

} // namespace ari::net
