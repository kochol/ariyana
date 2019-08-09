#pragma once
#include "Messages.hpp"

namespace ari::net
{
	class ServerSystem;

	// the adapter
	class GameAdapter : public yojimbo::Adapter {
	public:
		explicit GameAdapter(ServerSystem* server = nullptr) :
			m_server(server) {}

		yojimbo::MessageFactory* CreateMessageFactory(yojimbo::Allocator& allocator) override {
			return YOJIMBO_NEW(allocator, GameMessageFactory, allocator);
		}

		void OnServerClientConnected(int clientIndex) override;

		void OnServerClientDisconnected(int clientIndex) override;

	private:
		ServerSystem* m_server;
	};
} // namespace ari::net
