#include "ServerSystem.hpp"
#include "core/Assertion.hpp"
#include "core/memory/Memory.hpp"
#include "core/log.h"
#include "en/World.hpp"

namespace ari::net
{
	extern int g_iLastRpcClientIndex;

	//------------------------------------------------------------------------------
	ServerSystem::ServerSystem()
	{
		m_network_type = SystemNetworkType::Server;
	}

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
		_world->Subscribe<en::events::OnComponentAssigned<PropertyReplicator>>(this);
		_world->Subscribe<en::events::OnComponentRemoved<PropertyReplicator>>(this);
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

		if (_state == en::UpdateState::MainThreadState)
		{
			m_time += _elapsed;
			m_pServer->AdvanceTime(m_time);
			m_pServer->ReceivePackets();

			// process packets
			for (int i = 0; i < MAX_PLAYERS; i++) {
				if (m_pServer->IsClientConnected(i)) {
					for (int j = 0; j < m_connectionConfig.numChannels; j++) {
						yojimbo::Message* message = m_pServer->ReceiveMessage(i, j);
						while (message != NULL) {
							ProcessMessage(i, message);
							m_pServer->ReleaseMessage(i, message);
							message = m_pServer->ReceiveMessage(i, j);
						}
					}
				}
			}
		}
		else if (_state == en::UpdateState::SceneState)
		{
			// Send replicator messages
			if (m_iClientCount > 0)
			{
				for (auto pr : m_aPropertyReplicators)
				{
					for (auto& cmp : pr->Properties)
					{
						if (cmp.isDiffFn(cmp.PropertyClone, cmp.Component.Component, cmp.Index))
						{
							for (int i = 0; i < MAX_PLAYERS; i++) {
								if (m_pServer->IsClientConnected(i)) {
									auto msg = (UpdateEntityMessage*)m_pServer->CreateMessage
									(i, int(GameMessageType::UPDATE_ENTITY));
									msg->CmpHandle = cmp.Component.Handle;
									msg->CmpId = cmp.ComponentId;
									msg->MemberIndex = cmp.Index;
									msg->Component = cmp.Component.Component;
									m_pServer->SendMessage(i, int(GameChannel::UNRELIABLE), msg);
								}
							}
						}
					}
				}
			}

			m_pServer->SendPackets();
		}
	}

	//------------------------------------------------------------------------------
	bool ServerSystem::NeedUpdateOn(en::UpdateState::Enum _state)
	{
		return _state == en::UpdateState::MainThreadState || _state == en::UpdateState::SceneState;
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

	void ServerSystem::SaveReply(char* file_name)
	{
		m_bSaveReply = true; 
		m_sReplyFileName = file_name;
	}

	void ServerSystem::StopServer()
	{
		m_time = 0;
		m_iClientCount = 0;
		m_bSaveReply = false;
		m_sReplyFileName.Clear();
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
		log_info("A new client connected. client id = %d", client_id);

		// save to replay buffer
		if (m_bSaveReply)
		{
			int8_t msg_id = -2; // -2 for client gets connected
			m_bReplayBuffer.Add((uint8_t*)&msg_id, 1);
			uint8_t c_id = uint8_t(client_id);
			m_bReplayBuffer.Add(&c_id, 1);
			// save the time
			m_bReplayBuffer.Add((uint8_t*)&m_time, 8);
		}

		for (auto& e : m_aEntities)
		{
			auto msg = (CreateEntityMessage*)m_pServer->CreateMessage(client_id, int(GameMessageType::CREATE_ENTITY));
			msg->Entity = e;
			msg->World = m_pWorld;
			m_pServer->SendMessage(client_id, int(GameChannel::RELIABLE), msg);
		}
		m_pWorld->emit<en::events::OnClientConnected>({ client_id });
	}

	//------------------------------------------------------------------------------
	void ServerSystem::ClientDisconnected(int client_id)
	{
		m_iClientCount--;
		log_info("Client with id %d has been disconnected.", client_id);

		// save to replay buffer
		if (m_bSaveReply)
		{
			int8_t msg_id = -3; // -3 for client gets disconnected
			m_bReplayBuffer.Add((uint8_t*)&msg_id, 1);
			uint8_t c_id = uint8_t(client_id);
			m_bReplayBuffer.Add(&c_id, 1);
			// save the time
			m_bReplayBuffer.Add((uint8_t*)&m_time, 8);
		}

		m_pWorld->emit<en::events::OnClientDisconnected>({ client_id });
	}

	//------------------------------------------------------------------------------
	void ServerSystem::Receive(en::World* world, const en::events::OnEntityCreated& event)
	{
		if (event.entity->bReplicates)
		{
			m_aEntities.Add(event.entity);
			if (m_iClientCount == 0)
				return;

			// Send the entity to the clients
			for (int i = 0; i < MAX_PLAYERS; i++) {
				if (m_pServer->IsClientConnected(i)) {
					auto msg = (CreateEntityMessage*)m_pServer->CreateMessage(i, int(GameMessageType::CREATE_ENTITY));
					msg->Entity = event.entity;
					msg->World = m_pWorld;
					m_pServer->SendMessage(i, int(GameChannel::RELIABLE), msg);
				}
			}
		}
	}

	//------------------------------------------------------------------------------
	void ServerSystem::Receive(en::World* world, const en::events::OnEntityDestroyed& event)
	{
		if (event.entity->bReplicates)
			for(int i = 0; i < m_aEntities.Size(); i++)
				if (m_aEntities[i].Handle == event.entity.Handle)
				{
					// TODO: Send to the clients that they need to destroy an entity
					m_aEntities.Erase(i);
					return;
				}
	}

	//------------------------------------------------------------------------------
	void ServerSystem::Receive(en::World* world, const en::events::OnComponentAssigned<PropertyReplicator>& event)
	{
		m_aPropertyReplicators.Add(event.component);
	}

	//------------------------------------------------------------------------------
	void ServerSystem::Receive(en::World* world, const en::events::OnComponentRemoved<PropertyReplicator>& event)
	{
		for (int i = 0; i < m_aPropertyReplicators.Size(); ++i)
		{
			if (m_aPropertyReplicators[i] == event.component)
			{
				m_aPropertyReplicators.EraseSwap(i);
				return;
			}
		}
	}

	//------------------------------------------------------------------------------
	void ServerSystem::Call_C_RPC(void* rpc, bool _reliable, uint32_t _index, RpcType _rpc_type, int client_id)
	{
		if (m_iClientCount == 0)
			return;

		GameChannel channel = _reliable ? GameChannel::RELIABLE : GameChannel::UNRELIABLE;

		// Save the replay data
		int bytesNeeded = 12; /* 1 byte for rpc type 
							   + 1 byte for client count if it is a multi cast otherwise it is client id
							   + 1 byte for message type
							   + 8 bytes for time */
		CRpcCallMessage* replay_msg = nullptr;
		static core::Array<uint8_t> clients;

		if (m_bSaveReply)
			clients.Clear();

		if (_rpc_type == RpcType::MultiCast)
		{
			// Send to all clients
			CRpcCallMessage* msg = nullptr;
			for (int i = 0; i < MAX_PLAYERS; i++)
			{
				if (m_pServer->IsClientConnected(i))
				{
					if (msg)
						msg->AddRef();
					msg = (CRpcCallMessage*)m_pServer->CreateMessage
						(i, int(GameMessageType::CRPC_CALL));
					if (!replay_msg)
						replay_msg = msg;
					if (m_bSaveReply)
						clients.Add(uint8_t(i));

					msg->rpc = rpc;
					msg->rpc_index = _index;
					m_pServer->SendMessage(i, int(channel), msg);
				}
			}

			if (clients.Size() == 0)
				return;
		}
		else
		{
			a_assert(_rpc_type == RpcType::Client);
			if (m_pServer->IsClientConnected(client_id))
			{
				auto msg = (CRpcCallMessage*)m_pServer->CreateMessage
				(client_id, int(GameMessageType::CRPC_CALL));
				replay_msg = msg;
				msg->rpc = rpc;
				msg->rpc_index = _index;
				m_pServer->SendMessage(client_id, int(channel), msg);
			}
		}

		// Save the replay data
		if (m_bSaveReply)
		{
			// Save the replay to m_bReplayBuffer
			yojimbo::MeasureStream mStream(yojimbo::GetDefaultAllocator());
			if (replay_msg->SerializeInternal(mStream))
			{
				bytesNeeded += mStream.GetBytesProcessed() + clients.Size();
				bytesNeeded += bytesNeeded % 4 == 0 ? 0: 4 - bytesNeeded % 4;

 			    // Create write stream
				uint8_t* buffer = m_bReplayBuffer.Add(bytesNeeded);
				yojimbo::WriteStream wStream(
					yojimbo::GetDefaultAllocator(),
					buffer,
					bytesNeeded
					);

				auto save_replay = [&]()
				{
					// Save the rpc type
					int8_t rpc_type = int8_t(_rpc_type);
					serialize_bytes(wStream, (uint8_t*)&rpc_type, 1);

					if (clients.Size() > 0)
					{
						// Save the clients count and ids
						uint8_t client_count = uint8_t(clients.Size());
						serialize_bytes(wStream, &client_count, 1);
						for (int i = 0; i < clients.Size(); i++)
						{
							serialize_bytes(wStream, &clients[i], 1);
						}
					}
					else
					{
						// Save the client id
						uint8_t client_id_u8 = uint8_t(client_id);
						serialize_bytes(wStream, &client_id_u8, 1);
					}

					// Save the message type
					int8_t msgType = int8_t(GameMessageType::CRPC_CALL);
					serialize_bytes(wStream, (uint8_t*)&msgType, 1);
					// Save the time
					serialize_double(wStream, m_time);
					// Save the RPC
					return replay_msg->SerializeInternal(wStream);
				};
				save_replay();
			}
		}
	}

	//------------------------------------------------------------------------------
	void ServerSystem::SendRPC(RPC* rpc, int client_id)
	{
		if (m_iClientCount == 0)
			return;

		GameChannel channel = rpc->Reliable ? GameChannel::RELIABLE : GameChannel::UNRELIABLE;

		if (rpc->rpc_type == RpcType::MultiCast)
		{
			// Send to all clients
			for (int i = 0; i < MAX_PLAYERS; i++) 
			{
				if (m_pServer->IsClientConnected(i)) 
				{
					auto msg = (RpcCallMessage*)m_pServer->CreateMessage
						(i, int(GameMessageType::RPC_CALL));
					msg->rpc = rpc;
					m_pServer->SendMessage(i, int(channel), msg);
				}
			}
		}
		else
		{
			a_assert(rpc->rpc_type == RpcType::Client);
			if (m_pServer->IsClientConnected(client_id)) 
			{
				auto msg = (RpcCallMessage*)m_pServer->CreateMessage
					(client_id, int(GameMessageType::RPC_CALL));
				msg->rpc = rpc;
				m_pServer->SendMessage(client_id, int(channel), msg);
			}
		}
	}

	//------------------------------------------------------------------------------
	bool ServerSystem::ProcessMessage(int client_index, yojimbo::Message* msg)
	{
		if (msg->GetType() == int(GameMessageType::RPC_CALL))
		{
			auto rpc_msg = (RpcCallMessage*)msg;
			a_assert(rpc_msg->rpc->rpc_type == RpcType::Server);
			g_iLastRpcClientIndex = client_index;
			rpc_msg->rpc->Call();
		}
		else if (msg->GetType() == int(GameMessageType::CRPC_CALL))
		{
			auto rpc_msg = (CRpcCallMessage*)msg;
			g_iLastRpcClientIndex = client_index;
			g_on_call_rpc(rpc_msg->rpc_index);

			if (m_bSaveReply)
			{
				// Save the replay to m_bReplayBuffer
				yojimbo::MeasureStream mStream(yojimbo::GetDefaultAllocator());
				if (msg->SerializeInternal(mStream))
				{
					int bytesNeeded = mStream.GetBytesProcessed() 
						+ 12; /* 1 byte for rpc type
							   + 1 byte for client id;
							   + 1 byte for message type 
							   + 8 bytes for time */
						
					// Create write stream
					uint8_t* buffer = m_bReplayBuffer.Add(bytesNeeded);
					yojimbo::WriteStream wStream(
						yojimbo::GetDefaultAllocator(),
						buffer,
						bytesNeeded
						);

					// Save the rpc type
					int8_t rpc_type = -1; // It means it is an incoming msg
					serialize_bytes(wStream, (uint8_t*)&rpc_type, 1);
					// Save the client id
					uint8_t client_id = client_index;
					serialize_bytes(wStream, &client_id, 1);
					// Save the message type
					int8_t msgType = int8_t(GameMessageType::CRPC_CALL);
					serialize_bytes(wStream, (uint8_t*)&msgType, 1);
					// Save the time
					serialize_double(wStream, m_time);
					// Save the RPC
					msg->SerializeInternal(wStream);
				}
			}
		}

		return true;
	}

} // namespace ari::net
