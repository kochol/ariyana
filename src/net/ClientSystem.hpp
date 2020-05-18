#pragma once
#include "en/System.hpp"
#include <yojimbo.h>
#include "GameAdapter.hpp"
#include "NetworkSystem.hpp"

namespace ari::net
{
	class ClientSystem: public en::System, public NetworkSystem
	{
	public:

		ClientSystem();

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

		void AddComponent(const uint32_t& component_id, const uint32_t& component_server_handle,
			const uint32_t& component_client_handle);

		void* GetComponent(const uint32_t& component_id, const uint32_t& component_server_handle);

		void Call_C_RPC(void* rpc, bool _reliable, RpcType _rpc_type, int client_id);

	protected:

		void SendRPC(RPC* rpc, int client_id) override;

		void ProcessMessage(yojimbo::Message* msg);

		yojimbo::Client		*	m_pClient	= nullptr;
		core::Map<uint32_t // Server entity handle
			, uint32_t/* Client entity handle*/> m_mEntities;
		core::Map<uint32_t // Component ID
			, core::Map<uint32_t/* Server component handle*/
			, uint32_t /* Client component handle */>> m_mComponents;
	};
} // namespace ari::net
