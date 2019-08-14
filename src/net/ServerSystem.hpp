#pragma once
#include "en/System.hpp"
#include "yojimbo.h"
#include "GameAdapter.hpp"
#include "en/EventSubscriber.hpp"
#include "core/containers/Array.hpp"

namespace ari::net
{
	static const uint8_t DEFAULT_PRIVATE_KEY[yojimbo::KeyBytes] = { 0 };
	static const int MAX_PLAYERS = 64;

    class ServerSystem: public en::System,
		public en::EventSubscriber<en::events::OnEntityCreated>,
		public en::EventSubscriber<en::events::OnEntityDestroyed>,
		public en::EventSubscriber<en::events::OnComponentAssigned<PropertyReplicator>>,
		public en::EventSubscriber<en::events::OnComponentRemoved<PropertyReplicator>>
    {
    public:
		virtual ~ServerSystem();

		void Configure(en::World* _world) override;

		void Unconfigure(en::World* _world) override;

		void Update(en::World* _world, const float& _elapsed, en::UpdateState::Enum _state) override;

		bool NeedUpdateOn(en::UpdateState::Enum _state) override;

		bool CreateServer(const yojimbo::Address& address
			, const yojimbo::ClientServerConfig& config = GameConnectionConfig()
			, yojimbo::Adapter* adapter = nullptr);

		void StopServer();

		virtual void ClientConnected(int client_id);

		virtual void ClientDisconnected(int client_id);

		void Receive(en::World* world, const en::events::OnEntityCreated& event) override;

		void Receive(en::World* world, const en::events::OnEntityDestroyed& event) override;

		void Receive(en::World* world, const en::events::OnComponentAssigned<PropertyReplicator>& event) override;

		void Receive(en::World* world, const en::events::OnComponentRemoved<PropertyReplicator>& event) override;
		
    protected:

		en::World			*	m_pWorld	= nullptr;
		yojimbo::Server		*	m_pServer	= nullptr;
		yojimbo::Adapter	*	m_pAdapter	= nullptr;
		double					m_time		= 0.0;
		core::Array<en::EntityHandle>	m_aEntities;
		core::Array<PropertyReplicator*>
			m_aPropertyReplicators;
		int						m_iClientCount = 0;
		yojimbo::ClientServerConfig m_connectionConfig;
    };

} // namespace ari::net
