#include "ServerSystem.hpp"
#include "core/Assertion.hpp"
#include "core/memory/Memory.hpp"
#include "core/log.h"

namespace ari::net
{

	//------------------------------------------------------------------------------
	ServerSystem::~ServerSystem()
	{
		if (m_pAdapter)
			core::Memory::Delete(m_pAdapter);
	}

	//------------------------------------------------------------------------------
	void ServerSystem::Configure(en::World* _world)
	{
		m_pWorld = _world;
	}

	//------------------------------------------------------------------------------
	void ServerSystem::Unconfigure(en::World* _world)
	{
		m_pWorld = nullptr;
	}

	//------------------------------------------------------------------------------
	void ServerSystem::Update(en::World* _world, const float& _elapsed, en::UpdateState::Enum _state)
	{
		if (!m_pServer || !m_pServer->IsRunning())
			return;

		m_pServer->AdvanceTime(m_time + _elapsed);
		m_pServer->ReceivePackets();

		// TODO: process packets

		m_pServer->SendPackets();
	}

	//------------------------------------------------------------------------------
	bool ServerSystem::NeedUpdateOn(en::UpdateState::Enum _state)
	{
		return _state == en::UpdateState::SceneState;
	}

	//------------------------------------------------------------------------------
	bool ServerSystem::CreateServer(const yojimbo::Address& address
		, const yojimbo::ClientServerConfig& config, yojimbo::Adapter* adapter)
	{
		a_assert(!m_pServer);
		if (!adapter)
		{
			adapter = core::Memory::New<GameAdapter>(this);
			m_pAdapter = adapter;
		}
		m_pServer = core::Memory::New<yojimbo::Server>(yojimbo::GetDefaultAllocator(),
			DEFAULT_PRIVATE_KEY, address, config, *adapter, 0.0);
		m_pServer->Start(MAX_PLAYERS);
		if (!m_pServer->IsRunning()) {
			log_error("Could not start server at port %d", address.GetPort());
			return false;
		}

		// print the port we got in case we used port 0
		char buffer[256];
		m_pServer->GetAddress().ToString(buffer, sizeof(buffer));
		log_info("Server address is %s", buffer);

		return true;
	}

	//------------------------------------------------------------------------------
	void ServerSystem::ClientConnected(int client_id)
	{
		log_debug("A new client connected. client id = %d", client_id);
	}

	//------------------------------------------------------------------------------
	void ServerSystem::ClientDisconnected(int client_id)
	{
		log_debug("Client with id %d has been disconnected.", client_id);
	}

} // namespace ari::net
