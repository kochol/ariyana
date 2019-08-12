#pragma once
#include "Messages.hpp"

namespace ari::net
{
	class ServerSystem;

	// the adapter
	class GameAdapter : public yojimbo::Adapter {
	public:
		explicit GameAdapter(en::World* p_world, ServerSystem* server = nullptr) :
			m_server(server), m_pWorld(p_world) {}

		yojimbo::MessageFactory* CreateMessageFactory(yojimbo::Allocator& allocator) override {
			return YOJIMBO_NEW(allocator, GameMessageFactory, allocator, m_pWorld);
		}

		void OnServerClientConnected(int clientIndex) override;

		void OnServerClientDisconnected(int clientIndex) override;

	private:
		ServerSystem* m_server;
		en::World* m_pWorld;
	};
} // namespace ari::net
