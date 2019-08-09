#include "ClientSystem.hpp"
#include "ServerSystem.hpp"
#include "core/Assertion.hpp"
#include "core/memory/Memory.hpp"

namespace ari::net
{

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
	}

	//------------------------------------------------------------------------------
	void ClientSystem::Unconfigure(en::World* _world)
	{
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

		if (m_pClient->IsConnected()) {
			// ProcessMessages();

			// ... do connected stuff ...

			// send a message when space is pressed
		}

		m_pClient->SendPackets();
	}

	//------------------------------------------------------------------------------
	bool ClientSystem::NeedUpdateOn(en::UpdateState::Enum _state)
	{
		return _state == en::UpdateState::SceneState;
	}

	//------------------------------------------------------------------------------
	void ClientSystem::Connect(const yojimbo::Address& serverAddress
		, const yojimbo::ClientServerConfig& config, yojimbo::Adapter* adapter)
	{
		a_assert(!m_pClient);
		if (!adapter)
		{
			adapter = core::Memory::New<GameAdapter>(nullptr);
			m_pAdapter = adapter;
		}
		m_pClient = core::Memory::New<yojimbo::Client>(
			yojimbo::GetDefaultAllocator(), yojimbo::Address("0.0.0.0"), 
			config, *adapter, 0.0
			);
		uint64_t clientId;
		yojimbo::random_bytes((uint8_t*)& clientId, 8);
		m_pClient->InsecureConnect(DEFAULT_PRIVATE_KEY, clientId, serverAddress);
	}

	void ClientSystem::StopClient()
	{
		if (m_pClient)
		{
			m_pClient->Disconnect();
			core::Memory::Delete(m_pClient);
			m_pClient = nullptr;
		}
	}

} // namespace ari::net
