#include "ServerSystem.hpp"
#include "core/Assertion.hpp"
#include "core/memory/Memory.hpp"

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
	}

	//------------------------------------------------------------------------------
	bool ServerSystem::NeedUpdateOn(en::UpdateState::Enum _state)
	{
		return _state == en::UpdateState::SceneState;
	}

	//------------------------------------------------------------------------------
	void ServerSystem::CreateServer(const yojimbo::Address& address
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
	}

	//------------------------------------------------------------------------------
	void ServerSystem::ClientConnected(int client_id)
	{
	}

	//------------------------------------------------------------------------------
	void ServerSystem::ClientDisconnected(int client_id)
	{
	}
} // namespace ari::net
