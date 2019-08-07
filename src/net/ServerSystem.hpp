#pragma once
#include "en/System.hpp"
#include "yojimbo.h"
#include "GameAdapter.hpp"

namespace ari::net
{
	static const uint8_t DEFAULT_PRIVATE_KEY[yojimbo::KeyBytes] = { 0 };
	static const int MAX_PLAYERS = 64;

    class ServerSystem: public en::System
    {
    public:
		virtual ~ServerSystem() override;

		void Configure(en::World* _world) override;

		void Unconfigure(en::World* _world) override;

		void Update(en::World* _world, const float& _elapsed, en::UpdateState::Enum _state) override;

		bool NeedUpdateOn(en::UpdateState::Enum _state) override;

		bool CreateServer(const yojimbo::Address& address
			, const yojimbo::ClientServerConfig& config = GameConnectionConfig()
			, yojimbo::Adapter* adapter = nullptr);

		virtual void ClientConnected(int client_id);

		virtual void ClientDisconnected(int client_id);

    protected:

		en::World			*	m_pWorld	= nullptr;
		yojimbo::Server		*	m_pServer	= nullptr;
		yojimbo::Adapter	*	m_pAdapter	= nullptr;
    };

} // namespace ari::net
