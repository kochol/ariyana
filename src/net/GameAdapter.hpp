#pragma once
#include "Messages.hpp"

namespace ari::net
{
	class ServerSystem;
	class ClientSystem;

	// the adapter
	class GameAdapter : public yojimbo::Adapter {
	public:
		explicit GameAdapter(en::World* p_world, ServerSystem* server = nullptr, ClientSystem* _client = nullptr) :
			m_server(server), m_pWorld(p_world), m_pClient(_client) {}

		yojimbo::MessageFactory* CreateMessageFactory(yojimbo::Allocator& allocator) override {
			return YOJIMBO_NEW(allocator, GameMessageFactory, allocator, m_pWorld, m_pClient);
		}

		void OnServerClientConnected(int clientIndex) override;

		void OnServerClientDisconnected(int clientIndex) override;

	private:
		ServerSystem* m_server;
		ClientSystem* m_pClient;
		en::World* m_pWorld;
	};
} // namespace ari::net
