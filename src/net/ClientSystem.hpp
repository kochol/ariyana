#pragma once
#include "en/System.hpp"
#include <yojimbo.h>
#include "GameAdapter.hpp"

namespace ari::net
{
	class ClientSystem: public en::System
	{
	public:

		~ClientSystem() override;

		void Configure(en::World* _world) override;

		void Unconfigure(en::World* _world) override;

		void Update(en::World* _world, const float& _elapsed, en::UpdateState::Enum _state) override;

		bool NeedUpdateOn(en::UpdateState::Enum _state) override;

		void Connect(const yojimbo::Address& serverAddress, 
			const yojimbo::ClientServerConfig& config = GameConnectionConfig()
			, yojimbo::Adapter* adapter = nullptr);

		void StopClient();

		en::EntityHandle GetEntity(const uint32_t& server_entity_handle);

		void AddEntity(const uint32_t& server_entity_handle,
			const uint32_t& client_entity_handle);

	protected:

		yojimbo::Client		*	m_pClient	= nullptr;
		yojimbo::Adapter	*	m_pAdapter	= nullptr;
		double					m_time		= 0.0;
		en::World			*	m_pWorld	= nullptr;
		yojimbo::ClientServerConfig m_connectionConfig;
		core::Map<uint32_t // Server entity handle
			, uint32_t/* Client entity handle*/> m_mEntities;
	};
} // namespace ari::net
