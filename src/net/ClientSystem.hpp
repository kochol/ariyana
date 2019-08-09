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

	protected:

		yojimbo::Client		*	m_pClient	= nullptr;
		yojimbo::Adapter	*	m_pAdapter	= nullptr;
		double					m_time		= 0.0;
	};
} // namespace ari::net
