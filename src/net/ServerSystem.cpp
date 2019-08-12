#include "ServerSystem.hpp"
#include "core/Assertion.hpp"
#include "core/memory/Memory.hpp"
#include "core/log.h"
#include "en/World.hpp"

namespace ari::net
{

	//------------------------------------------------------------------------------
	ServerSystem::~ServerSystem()
	{
		StopServer();
		if (m_pAdapter)
			core::Memory::Delete(m_pAdapter);
	}

	//------------------------------------------------------------------------------
	void ServerSystem::Configure(en::World* _world)
	{
		m_pWorld = _world;
		_world->Subscribe<en::events::OnEntityCreated>(this);
		_world->Subscribe<en::events::OnEntityDestroyed>(this);
	}

	//------------------------------------------------------------------------------
	void ServerSystem::Unconfigure(en::World* _world)
	{
		m_pWorld = nullptr;
		_world->unsubscribeAll(this);
	}

	//------------------------------------------------------------------------------
	void ServerSystem::Update(en::World* _world, const float& _elapsed, en::UpdateState::Enum _state)
	{
		if (!m_pServer || !m_pServer->IsRunning())
			return;

		m_time += _elapsed;
		m_pServer->AdvanceTime(m_time);
		m_pServer->ReceivePackets();

		// TODO: process packets
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (m_pServer->IsClientConnected(i)) {
				for (int j = 0; j < m_connectionConfig.numChannels; j++) {
					yojimbo::Message* message = m_pServer->ReceiveMessage(i, j);
					while (message != NULL) {
						//ProcessMessage(i, message);
						m_pServer->ReleaseMessage(i, message);
						message = m_pServer->ReceiveMessage(i, j);
					}
				}
			}
		}

		m_pServer->SendPackets();

	}

	//------------------------------------------------------------------------------
	bool ServerSystem::NeedUpdateOn(en::UpdateState::Enum _state)
	{
		return _state == en::UpdateState::MainThreadState;
	}

	//------------------------------------------------------------------------------
	bool ServerSystem::CreateServer(const yojimbo::Address& address
		, const yojimbo::ClientServerConfig& config, yojimbo::Adapter* adapter)
	{
		a_assert(!m_pServer);
		a_assert(m_pWorld); // Add system to the world first
		if (!adapter)
		{
			adapter = core::Memory::New<GameAdapter>(m_pWorld, this);
			m_pAdapter = adapter;
		}
		m_connectionConfig = config;
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

	void ServerSystem::StopServer()
	{
		if (m_pServer)
		{
			m_pServer->Stop();
			core::Memory::Delete(m_pServer);
			m_pServer = nullptr;
		}
	}

	//------------------------------------------------------------------------------
	void ServerSystem::ClientConnected(int client_id)
	{
		m_iClientCount++;
		log_debug("A new client connected. client id = %d", client_id);
		for (auto& e : m_aEntities)
		{
			auto msg = (CreateEntityMessage*)m_pServer->CreateMessage(client_id, int(GameMessageType::CREATE_ENTITY));
			msg->Entity = e;
			msg->World = m_pWorld;
			m_pServer->SendMessage(client_id, int(GameChannel::RELIABLE), msg);
		}
	}

	//------------------------------------------------------------------------------
	void ServerSystem::ClientDisconnected(int client_id)
	{
		m_iClientCount--;
		log_debug("Client with id %d has been disconnected.", client_id);
	}

	//------------------------------------------------------------------------------
	void ServerSystem::Receive(en::World* world, const en::events::OnEntityCreated& event)
	{
		if (world->GetEntity(event.entity)->bReplicates)
		{
			m_aEntities.Add(event.entity);
			if (m_iClientCount == 0)
				return;

			// Send the entity to the clients
			auto msg = (CreateEntityMessage*)m_pServer->CreateMessage(0, int(GameMessageType::CREATE_ENTITY));
			msg->Entity = event.entity;
			msg->World = m_pWorld;
			SendToAll(int(GameChannel::RELIABLE), msg);
		}
	}

	//------------------------------------------------------------------------------
	void ServerSystem::Receive(en::World* world, const en::events::OnEntityDestroyed& event)
	{
		if (world->GetEntity(event.entity)->bReplicates)
			for(int i = 0; i < m_aEntities.Size(); i++)
				if (m_aEntities[i].Handle == event.entity.Handle)
				{
					// TODO: Send to the clients that they need to destroy an entity
					m_aEntities.Erase(i);
					return;
				}
	}

	void ServerSystem::SendToAll(int channel_id, yojimbo::Message* msg)
	{
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (m_pServer->IsClientConnected(i)) {
				m_pServer->SendMessage(i, channel_id, msg);
			}
		}
	}

} // namespace ari::net
