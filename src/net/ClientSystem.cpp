#include "ClientSystem.hpp"
#include "ServerSystem.hpp"
#include "core/Assertion.hpp"
#include "core/memory/Memory.hpp"

namespace ari::net
{
	//------------------------------------------------------------------------------
	ClientSystem::ClientSystem()
	{
		m_network_type = SystemNetworkType::Client;
	}

	//------------------------------------------------------------------------------
	ClientSystem::~ClientSystem()
	{
		StopClient();
		if (m_pAdapter)
			core::Memory::Delete(m_pAdapter);
	}

	//------------------------------------------------------------------------------
	void ClientSystem::Configure(en::World* _world)
	{
		m_pWorld = _world;
	}

	//------------------------------------------------------------------------------
	void ClientSystem::Unconfigure(en::World* _world)
	{
		m_pWorld = nullptr;
	}

	//------------------------------------------------------------------------------
	void ClientSystem::Update(en::World* _world, const float& _elapsed, en::UpdateState::Enum _state)
	{
		if (!m_pClient)
			return;

		// update client
		m_time += _elapsed;
		m_pClient->AdvanceTime(m_time);
		m_pClient->ReceivePackets();

		if (m_pClient->IsConnected()) 
		{
			for (int i = 0; i < m_connectionConfig.numChannels; i++) {
				yojimbo::Message* message = m_pClient->ReceiveMessage(i);
				while (message != NULL) {
					ProcessMessage(message);
					m_pClient->ReleaseMessage(message);
					message = m_pClient->ReceiveMessage(i);
				}
			}
		}

		m_pClient->SendPackets();
	}

	//------------------------------------------------------------------------------
	bool ClientSystem::NeedUpdateOn(en::UpdateState::Enum _state)
	{
		return _state == en::UpdateState::MainThreadState;
	}

	//------------------------------------------------------------------------------
	void ClientSystem::Connect(const yojimbo::Address& serverAddress
		, const yojimbo::ClientServerConfig& config, yojimbo::Adapter* adapter)
	{
		a_assert(!m_pClient);
		a_assert(m_pWorld); // Add system to the world first
		if (!adapter)
		{
			adapter = core::Memory::New<GameAdapter>(m_pWorld, nullptr, this);
		}
		m_pAdapter = adapter;
		m_connectionConfig = config;
		m_pClient = core::Memory::New<yojimbo::Client>(
			yojimbo::GetDefaultAllocator(), yojimbo::Address("0.0.0.0"), 
			config, *adapter, 0.0
			);
		uint64_t clientId;
		yojimbo::random_bytes((uint8_t*)& clientId, 8);
		m_pClient->InsecureConnect(DEFAULT_PRIVATE_KEY, clientId, serverAddress);
	}

	//------------------------------------------------------------------------------
	void ClientSystem::StopClient()
	{
		if (m_pClient)
		{
			m_pClient->Disconnect();
			core::Memory::Delete(m_pClient);
			m_pClient = nullptr;
		}
	}

	//------------------------------------------------------------------------------
	en::EntityHandle ClientSystem::GetEntity(const uint32_t& server_entity_handle)
	{
		int found = m_mEntities.FindIndex(server_entity_handle);
		if (found == core::InvalidIndex)
			return en::EntityHandle();
		uint32_t h = m_mEntities.ValueAtIndex(found);
		return { h , core::HandleManager<en::EntityHandle>::FindIndex(h) };
	}

	//------------------------------------------------------------------------------
	void ClientSystem::AddEntity(const uint32_t& server_entity_handle, const uint32_t& client_entity_handle)
	{
		m_mEntities.Add(server_entity_handle, client_entity_handle);
	}

	//------------------------------------------------------------------------------
	void ClientSystem::AddComponent(const uint32_t& component_id, const uint32_t& component_server_handle, const uint32_t& component_client_handle)
	{
		if (!m_mComponents.Contains(component_id))
			m_mComponents.Add(component_id, core::Map<uint32_t, uint32_t>());
		m_mComponents[component_id].Add(component_server_handle, component_client_handle);
	}

	//------------------------------------------------------------------------------
	void* ClientSystem::GetComponent(const uint32_t& component_id, const uint32_t& component_server_handle)
	{
		uint32_t cmp_handle = m_mComponents[component_id][component_server_handle];
		return m_pWorld->GetComponent(component_id, cmp_handle);
	}

	//------------------------------------------------------------------------------
	void ClientSystem::Call_C_RPC(void* rpc, bool _reliable, uint32_t _index, RpcType _rpc_type, int client_id)
	{
		a_assert(_rpc_type == RpcType::Server);

		if (!m_pClient)
			return;

		if (!m_pClient->IsConnected())
			return;

		GameChannel channel = _reliable ? GameChannel::RELIABLE : GameChannel::UNRELIABLE;
		auto msg = (CRpcCallMessage*)m_pClient->CreateMessage(int(GameMessageType::CRPC_CALL));
		msg->rpc = rpc;
		msg->rpc_index = _index;
		m_pClient->SendMessage(int(channel), msg);
	}

	//------------------------------------------------------------------------------
	void ClientSystem::SendRPC(RPC* rpc, int client_id)
	{
		a_assert(rpc->rpc_type == RpcType::Server);

		if (!m_pClient->IsConnected())
			return;

		GameChannel channel = rpc->Reliable ? GameChannel::RELIABLE : GameChannel::UNRELIABLE;
		auto msg = (RpcCallMessage*)m_pClient->CreateMessage(int(GameMessageType::RPC_CALL));
		msg->rpc = rpc;
		m_pClient->SendMessage(int(channel), msg);
	}

	//------------------------------------------------------------------------------
	void ClientSystem::ProcessMessage(yojimbo::Message* msg)
	{
		if (msg->GetType() == int(GameMessageType::RPC_CALL))
		{
			auto rpc_msg = (RpcCallMessage*)msg;
			rpc_msg->rpc->Call();
		}
		else if (msg->GetType() == int(GameMessageType::CRPC_CALL))
		{
			auto rpc_msg = (CRpcCallMessage*)msg;
			g_on_call_rpc(rpc_msg->rpc_index);
		}
	}

} // namespace ari::net
